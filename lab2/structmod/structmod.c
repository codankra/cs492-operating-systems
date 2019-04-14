#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/random.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/kfifo.h>
#include <linux/moduleparam.h>
#include <linux/idr.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#define DRIVER_AUTHOR "Daniel Kramer <dkramer@stevens.edu>"
#define DRIVER_DESC "Lab2"

int dstruct_size = 0;
module_param(dstruct_size, int, 0);

struct k_list{
	struct list_head test_list;
	int temp;
};
/* LINKED LIST 1 */
void ll1(int sized){
	struct k_list *entry;
	struct list_head *ptr;
	struct list_head *ptr_bkp;
	LIST_HEAD(test_head);
	//adding
	int i  = 0;
	struct k_list* add;
	int rand;
	while (i<sized){
		add = kmalloc(sizeof(struct k_list), GFP_KERNEL);
		get_random_bytes(&rand, sizeof(int));
		add->temp = rand;
		INIT_LIST_HEAD(&add->test_list);
		list_add(&add->test_list,&test_head);
		i++;
	}
	//printk(KERN_ALERT "Safe\n");
	list_for_each(ptr,&test_head){
		entry = list_entry(ptr,struct k_list,test_list);
		printk(KERN_INFO "Hello %d\n", entry->temp);
	}
	//printk(KERN_ALERT "Safe\n");
	list_for_each_safe(ptr, ptr_bkp, &test_head){
		entry = list_entry(ptr, struct k_list,test_list);
		printk(KERN_INFO "Deleting %d\n", entry->temp);
		list_del(&entry->test_list);
	}
	//printk(KERN_ALERT "Safe\n");
	return;
}

/* LINKED LIST 2 */
void ll2del(struct list_head *test_head){
	struct list_head *ptr;
	struct list_head *ptr_bkp;
	struct k_list *entry;
	list_for_each_safe(ptr, ptr_bkp, test_head){
		entry = list_entry(ptr, struct k_list,test_list);
		printk(KERN_INFO "LL2: Deleting %d\n", entry->temp);
		list_del(&entry->test_list);
	}
	//printk(KERN_ALERT "Safe\n");
	return;
}
void ll2iter(struct list_head *test_head){
	struct list_head *ptr;
	struct k_list *entry;
	list_for_each(ptr,test_head){
		entry = list_entry(ptr,struct k_list,test_list);
		printk(KERN_INFO "LL2: Hello %d\n", entry->temp);
	}
	//printk(KERN_ALERT "Safe\n");
	ll2del(test_head);
}
void ll2start(int sized){
	LIST_HEAD(test_head);
	int i = 0;
	struct k_list* add;
	int rand;
	while (i<sized){
		add = kmalloc(sizeof(struct k_list), GFP_KERNEL);
		get_random_bytes(&rand, sizeof(int));
		add->temp = rand;
		INIT_LIST_HEAD(&add->test_list);
		list_add(&add->test_list,&test_head);
		i++;
	}
	//printk(KERN_ALERT "Safe\n");
	ll2iter(&test_head);
}
/* QUEUE */
void queue(int sized){
	unsigned int MAX_SIZE = 256;
	DECLARE_KFIFO(my_fifo, int, MAX_SIZE);
	INIT_KFIFO(my_fifo);
	//printk(KERN_ALERT "Safe\n");
	int qrand;
	int i = 0;
	while (i<sized){
		//create random thing from, pass it in
		get_random_bytes(&qrand, sizeof(int));
		printk(KERN_INFO "Queue: Inserting %d\n", qrand);
		kfifo_put(&my_fifo, qrand);
		i++;
	}
	//printk(KERN_ALERT "Safe\n");
	i = 0;
	while (i<sized){
		kfifo_get(&my_fifo, &qrand);
		printk(KERN_INFO "Queue: Deleting %d\n", qrand);
		i++;
	}
	//kfifo_free(my_fifo);
	//printk(KERN_ALERT "Safe\n");
	return;
}

/* MAP */
void map(int sized){
	DEFINE_IDR(my_idr);
	int LOWBOUND = 0;
	int HIGHBOUND = 32;
	int i = 0;
	int* idrand;
	while (i<sized){
		idrand = kmalloc(sizeof(int), GFP_KERNEL);
		get_random_bytes(idrand, sizeof(int));
		idr_preload(GFP_KERNEL);
		idr_alloc(&my_idr, idrand, LOWBOUND, HIGHBOUND, GFP_KERNEL);
		idr_preload_end();
		i++;
	}
	//printk(KERN_ALERT "Safe\n");
	int entryid= 0;
	int* entryval;
//	while(entryid<32){
//		entryval = idr_get_next(&my_idr, &entryid);
//		printk(KERN_INFO "(MAP) Id: %d , Entry: %d\n", entryid, &entryval);
//		entryid++;
//	}
	idr_for_each_entry(&my_idr, entryval, entryid){
		printk(KERN_INFO "(MAP) Id: %d , Entry: %d\n", entryid, *entryval);
	}
	//printk(KERN_ALERT "Safe\n");
	return;
}


/* RB Tree */
struct my_rb{
	struct rb_node node;
	int keystring; //"aptly" named but easy to remember
	int val;
};


struct my_rb *my_search(struct rb_root *root, int intval) {
	struct rb_node *node = root->rb_node;
	while(node){
		struct my_rb *data = container_of(node, struct my_rb, node);
		int result;
		result = intval - data->keystring;
		if (result < 0)
			node = node->rb_left;
		else if (result > 0)
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}
int my_insert(struct rb_root *root, struct my_rb *data){
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	//place node
	while(*new) {
		struct my_rb *this = container_of(*new, struct my_rb, node);
		int result = data->keystring - this->keystring;
		parent = *new;
		if (result < 0)
			new = &((*new)->rb_left);
		else if (result > 0)
			new = &((*new)->rb_right);
		else
			return -1;
	}
	//add node/rebalance tree
	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);
	return 0;

}

void rbtree(int sized){
	//search and insert defined above
	struct rb_root root = RB_ROOT;
	int rbrand;
	struct my_rb *data;
	int i = 0;
	while (i<sized){
		data = kmalloc(sizeof(struct my_rb), GFP_KERNEL);
		get_random_bytes(&rbrand, sizeof(int));
		//no duplicates because very low chance
		data->keystring = i;
		data->val = rbrand;
		my_insert(&root, data);
		i++;
	}
	i = 0;
	//printk(KERN_ALERT "Safe\n");
	while (i<sized){
		data = my_search(&root, i);
		printk(KERN_INFO "Red_Black_Tree: ID %d: %d\n", i, data->val);
		i++;
	}
	//printk(KERN_ALERT "Safe\n");
	int lowerRange;
	int higherRange;
	get_random_bytes(&lowerRange, sizeof(int));
	get_random_bytes(&higherRange, sizeof(int));
	lowerRange = lowerRange % 19;
	higherRange = higherRange % 19;
	if(higherRange<0) higherRange = 0 - higherRange;
	if(lowerRange<0) lowerRange = 0 - lowerRange;
	if(lowerRange > higherRange){
		i = higherRange;
		higherRange = lowerRange;
		lowerRange = i;
	}
	i = lowerRange;
	printk(KERN_INFO "Searching between %d and %d...\n", lowerRange, higherRange);
	while(i<=higherRange){
		data = my_search(&root, i);
		if(data){
			printk(KERN_INFO "Red_Black_Tree: %d found at position %d: Removing.\n", data->val, i);
			rb_erase(&data->node, &root);
		}
		i++;
	}
	//printk(KERN_ALERT "Safe\n");
	i = (0 == lowerRange) ? higherRange+1 : 0;
	//final print and remove
	while(i<sized){
		data = my_search(&root, i);
		printk(KERN_INFO "Red_Black_Tree: ID %d: %d\n", i, data->val);
		rb_erase(&data->node, &root);
		i++;
		if(i == lowerRange)
			i = higherRange + 1;
	}
	return;

}
int init_module(void){
	printk(KERN_INFO "Hello world with dstruct_size = %d\n", dstruct_size);
	ll1(dstruct_size);
	ll2start(dstruct_size);
	queue(dstruct_size);
	map(dstruct_size);
	rbtree(dstruct_size);
	return 0;
}

void cleanup_module(void){
	printk(KERN_INFO "Goodbye world I am done\n");
	return;
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
