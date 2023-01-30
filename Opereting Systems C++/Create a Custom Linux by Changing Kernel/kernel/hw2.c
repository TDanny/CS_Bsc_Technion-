#include <linux/kernel.h>
#include<linux/sched.h>


asmlinkage long sys_set_weight(int weight){
        if(weight < 0){
                return -EINVAL;
        }
        current->weight = weight; 
        return 0;
}

asmlinkage long sys_get_weight(void){
        //simple function thast returns the wegiht of the current procces
        return current->weight;
}

long sys_get_leaf_children_sum_helper(struct task_struct* task){
        long weight_total = 0;
  	struct list_head* node;
        struct task_struct* my_current_task;
        if(list_empty(&task->children)){
                return task->weight;
        }
        list_for_each(node, &task->children){
                my_current_task = list_entry(node, struct task_struct, sibling);
                weight_total += sys_get_leaf_children_sum_helper(my_current_task);
        }

        return weight_total;
}


asmlinkage long sys_get_leaf_children_sum(void){
        if(list_empty(&current->children))
        {
                return -ECHILD;
        }
        return sys_get_leaf_children_sum_helper(current);

}

asmlinkage long sys_hello(void) 
{
        printk("Hello, World!\n");
        return 0;
} 



asmlinkage long sys_get_heaviest_ancestor(void){

        long maximun_pid = current->pid;
        long maximun_weight = current->weight;

        struct task_struct* my_current_task = current;
        while(my_current_task->pid != 1)
        {
                my_current_task = my_current_task->real_parent;
                if(my_current_task->weight > maximun_weight){
                        maximun_pid = my_current_task->pid;
                        maximun_weight = my_current_task->weight;
                }
        }

        return maximun_pid;
}