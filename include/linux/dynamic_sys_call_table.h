#ifndef _DYNAMIC_SYS_CALL_TABLE_H_
#define _DYNAMIC_SYS_CALL_TABLE_H_

#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
#include <asm/syscall.h>
#include <linux/binfmts.h>

#include <linux/list.h>

struct sys_call_array_container { 
    char platform[30];
    // binary handler
    struct linux_binfmt *bin_handler;
    // system call table
    sys_call_ptr_t *tbl_ptr;

    struct list_head lst;
};

// Add system call table to list
void sys_tbl_init_list(void);
void sys_tbl_set_default_table(sys_call_ptr_t *table);
sys_call_ptr_t *sys_tbl_get_default_table(void);

void sys_tbl_add(sys_call_ptr_t *tbl_ptr , const char *platform);
bool sys_tbl_set_binary_handler(struct linux_binfmt *bin_handler , const char *platform);
sys_call_ptr_t *sys_tbl_search_by_bin_handler(struct linux_binfmt *bin_handler);
sys_call_ptr_t *sys_tbl_search_by_platform(const char *platform);

// Switch the global system call table and return the old system call
void sys_tbl_set_global_table(sys_call_ptr_t *new_table);

// architecture-dependent
void sys_tbl_register_systbls(void);

#endif