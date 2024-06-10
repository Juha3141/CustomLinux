#include <linux/dynamic_sys_call_table.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#include <linux/printk.h>

// Declare list here!
static LIST_HEAD(sys_call_list);
static sys_call_ptr_t *default_sys_call_tbl;

extern asmlinkage sys_call_ptr_t *sys_call_table;

void __init sys_tbl_init_list(void) {
    INIT_LIST_HEAD(&sys_call_list);
    
    // register system call tables (architecture-specific function)
    sys_tbl_register_systbls();
}

void sys_tbl_set_default_table(sys_call_ptr_t *table) { default_sys_call_tbl = table; }
sys_call_ptr_t *sys_tbl_get_default_table(void) { return default_sys_call_tbl; }

// add the system call table to the list
void sys_tbl_add(sys_call_ptr_t *tbl_ptr , const char *platform) {
    struct sys_call_array_container *new_container = kmalloc(sizeof(struct sys_call_array_container) , GFP_KERNEL);
    new_container->tbl_ptr = tbl_ptr;
    new_container->bin_handler = 0x00;
    strcpy(new_container->platform , platform); // possibly dangerous!
    list_add(&new_container->lst , &sys_call_list);
}

bool sys_tbl_set_binary_handler(struct linux_binfmt *bin_handler , const char *platform) {
    struct sys_call_array_container *cur;
    list_for_each_entry(cur , &sys_call_list , lst) {
        if(strcmp(cur->platform , platform) == 0) {
            if(cur->bin_handler != 0x00) return false;
            cur->bin_handler = bin_handler;
            return true;
        }
    }
    return false;
}

sys_call_ptr_t *sys_tbl_search_by_bin_handler(struct linux_binfmt *bin_handler) {
    struct sys_call_array_container *cur;
    list_for_each_entry(cur , &sys_call_list , lst) {
        if(cur->bin_handler == bin_handler) {
            return cur->tbl_ptr;
        }
    }
    return 0x00;
}

sys_call_ptr_t *sys_tbl_search_by_platform(const char *platform) {
    struct sys_call_array_container *cur;
    list_for_each_entry(cur , &sys_call_list , lst) {
        if(strcmp(cur->platform , platform) == 0) {
            return cur->tbl_ptr;
        }
    }
    return 0x00;
}

void sys_tbl_set_global_table(sys_call_ptr_t *new_table) {
    if(new_table == 0x00) { return; }
    if(new_table == sys_tbl_search_by_platform("myelf")) {
        printk(KERN_NOTICE"registering the global table to myelf , 0x%lx\n" , (unsigned long)new_table);
    }
    sys_call_table = new_table;
}

asmlinkage long __x64___sys_get_current_syscall_tbl(void) {
    printk("(syscall) current system call table : 0x%lx\n" , (unsigned long)sys_call_table);
    return 0x00;
}

EXPORT_SYMBOL_GPL(sys_tbl_set_binary_handler);
EXPORT_SYMBOL_GPL(sys_tbl_search_by_bin_handler);
EXPORT_SYMBOL_GPL(sys_tbl_search_by_platform);