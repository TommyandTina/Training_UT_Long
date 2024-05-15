/**
 * @file io_manager.h
 * @brief OSAL IO Manager evaluation header file.
 * @author K.Takagi
 * @date 2019/11/12
 */

#ifndef IO_MANAGER_H
#define IO_MANAGER_H

typedef struct st_osal_test_info{
    char* device_type;
    size_t numOfDevices;
    size_t numOfList;
    char* list;
} st_osal_test_info_t;

/* prototype */
void osal_test_io_manager_display_menu();
void osal_test_io_manager_menu(int child);
#endif
