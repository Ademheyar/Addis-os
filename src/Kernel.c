#include <Addis/Drivers/BIOSINFO/Multiboot.h>
#include <Kernel.h>
#include <Addis/Tables/WorkingT.h>
#include <Addis/Drivers/Screen/Vesa/Vesa.h>

#include <Addis/Drivers/Filse_system/Fat/Fat.h>
#include <Drivers/Serial/Serial.h>
#include <Addis/Interrupt/Idt.h>
#include <Addis/Read_Do.h>
#include <Addis/Drivers/Screen/Vga/Vga.h>
#include <Addis/Libs/String/Text.h>
#include <Addis/Readcode.h>
#include <Shell.h>
#include <x86.h>


// Entry point for the kernel
void kmain()
{
    // Clear the screen
    clearScreen();

    // Print a welcome message to the user
    print("Hi and Welcome to NIDOS operating system\nPlease enter a command\n");

    // Launch the shell with an initial state of 0
    launch_shell(0);    
}

// Function to handle idle state of the kernel
void idle() {
    while(1) {
        // Log debug information before halting the CPU
        DEBUG("in x86_hlt\n");

        // Halt the CPU until the next interrupt
        x86_hlt();

        // Log debug information after halting the CPU
        DEBUG("out x86_hlt\n");
    }
}

// Function to handle reading and processing tasks
void Read_focused()
{ 
    // Check if the current task has a reading task assigned
    if (task_list_current->holded_info->read->fread) 
    {
        // Log debug information about the task being read
        DEBUG("GOING %d TO READ TASK %d CODE\n\n\n", task_list_current->state, task_list_current->id);

        do {
            // Log debug information about the reading process
            DEBUG("going ");

            // Check if the current task is in a specific state and update the reading task
            if (issame(task_list_current->holded_info->read->fread->reading_for[task_list_current->holded_info->read->fread->rfor_id], "WDEF") &&
                task_list_current->holded_info->read->read_next && task_list_current->holded_info->read->read_next->fread->read_new)
                task_list_current->holded_info->read = task_list_current->holded_info->read->read_next;

            // If there is new code to read, process it
            if(task_list_current->holded_info->read->fread->read_new) {
                DEBUG("to fix Code(%s) Going to read Bodys\n", task_list_current->holded_info->read->fread->read_new);
                // Read the body of the code
                char *rbcode = read_bodys(task_list_current->holded_info->read->fread->read_new);
                DEBUG("Done Reading Bodys fixed Code(%s)....\n", rbcode);

                // If the code is valid, process it further
                if(rbcode && (!issame(rbcode, "") || !issame(rbcode, " ")))
                {
                    // Update the reading task with the processed code
                    task_list_current->holded_info->read->fread->main_readcode = stradd(rbcode," ", 0);
                    task_list_current->holded_info->read->fread->reading_value = str_splitL(stradd(rbcode," ", 0), " ", 0);
                    task_list_current->holded_info->read->fread->read_new = 0;
                    task_list_current->holded_info->read->fread->reading_on = 0;
                    task_list_current->holded_info->read->fread->reading_stoped = 0;

                    // Create a new worktable for the task
                    Create_newworktable();

                    // Clear any previous worktables
                    Clear_worktables(-1);

                    // Log debug information about the completed code fixing
                    DEBUG(" done Pripering Working Table r %d %d  l %d v %s\n", task_list_current->holded_info->read->fread->reading_on, task_list_current->holded_info->read->fread->reading_stoped, (int)task_list_current->holded_info->read->fread->reading_value->length, rbcode);
                }
                // If no valid code is found, mark the reading as stopped
                else {
                    task_list_current->holded_info->read->fread->reading_stoped = (int)task_list_current->holded_info->read->fread->reading_value->length;
                    // Log debug information about the absence of code
                    DEBUG("There is no code to fix\n");
                    // Remove the reading task
                    Remove_READING();
                }
            }

            // Log debug information about the reading process
            DEBUG(" Reading The Code\n\n\n");

            // Check if there is any code to read
            if(task_list_current->holded_info->read->fread->reading_value && task_list_current->holded_info->read->fread->reading_value->length > 0){
                if((int)task_list_current->holded_info->read->fread->reading_value->length  - task_list_current->holded_info->read->fread->reading_stoped > 0) 
                {
                    // Log debug information about the code being read
                    DEBUG("length(%d) stoped(%d) reading code {\n%s\n}\n", task_list_current->holded_info->read->fread->reading_value->length, task_list_current->holded_info->read->fread->reading_stoped, task_list_current->holded_info->read->fread->main_readcode);        

                    // Perform the reading operation
                    // this will read the code and do what it says to do
                    ReadDo();

                    // Log debug information about the completed reading
                    DEBUG("done reading DO\n");

                    // Handle specific reading states and tasks
                    int rfid = 0;
                    if (task_list_current->holded_info->read->fread->rfor_id) rfid = task_list_current->holded_info->read->fread->rfor_id;

                    if(rfid >= 0)  {
                        // Handle specific reading states based on the task
                        // (Detailed handling logic is omitted for brevity)
                    }
                }
                // If no code is left to read, mark the reading as stopped
                else {
                    task_list_current->holded_info->read->fread->reading_stoped = (int)task_list_current->holded_info->read->fread->reading_value->length;
                    // Log debug information about the absence of code
                    DEBUG("There is no code to read\n");
                    // Remove the reading task
                    Remove_READING();
                }
            }
        } while(task_list_current->state == 3 || task_list_current->state >= 6);

        // Log debug information about the completed reading task
        DEBUG("DONE READING TASK %d CODE\n\n\n", task_list_current->id);
    }

    // Log debug information about the completed reading process
    DEBUG("done read code name(%s)\n", task_list_current->holded_info->fread->name);

    // Reload the current task to process it again
    reload_current_task();
}

// Function to load boot information and initialize tasks
void get_bootinfo()
{
    // Check if the current task has any information, if not, create a new one
    // this will check if there is any information for current task if not it will create new one
    if(task_list_current->holded_info) 
    {
        DEBUG("Going to create new readinginfo\n");
        // Create a new reading task
        // Create_readinfo('C'); --- IGNORE ---
        Create_readinfo('C'); 
        
        DEBUG("Goint to start reading from kernel \n");
        //int i ;
        int state = PROCESS_STATE_READ_TO_ONE_END;
        sysinfo.sys_mode = stradd("", "START", 0);
        
        DEBUG("Going to add first code from file boot start.oac /boot/Start.oac\n");
        // Get the code from the specified path
        char *code = get_code("/boot/Start.oac"); // the first code to read will be start.oac because it will prepare system to read other codes and it will be the main code for system
        // Log debug information about the code retrieval
        DEBUG(" done ");

        if (code != NULL) {
            // If the code is valid, create a new kernel process for it
            DEBUG("gatting code\n\n\n");
            create_kernel_process_last((void*)Read_focused, state);
            Create_readinfo('L');
            Create_working_place();

            // Log debug information about the code preparation
            DEBUG(" going to create and prepar to fix Code(%s)....\n", code);

            if (code && !(issame(code, "") || issame(code, " "))) {
                // Update the task with the retrieved code
                task_list_last->holded_info->read->fread->reading_task = task_list_last;
                //task_list_last->holded_info->read->fread->name = strdup(task_list_current->holded_info->read->bootinfo->code_splited1[i-1]);
                task_list_last->holded_info->read->fread->getas = "";
                task_list_last->holded_info->read->fread->getwhat = "";
                task_list_last->holded_info->read->fread->read_new = stradd(code," ", 0);
                task_list_last->holded_info->read->fread->rfor_id = 0;
                task_list_last->holded_info->read->fread->reading_for[0] = "READING";

                // Log debug information about the task creation
                DEBUG(" done creating(task-id[%d] by state[%d]) and fixing code %s\n", task_list_last->id, task_list_current->state, code);
            }
            else {
                // If the code is invalid, delete the last created working place
                DEBUG(" can't creating and fixing code else %s\n", code);
                Delete_lateworking_place();
                DEBUG(" done deleting \n");
            }
        }
        DEBUG("DONE Adding reading bootinfo\n\n\n");
    }
    else {
        // If there is already information for the current task, log debug information
        DEBUG("There is already readinginfo for current task\n");
    }
}

// we will staring coding in c starting from this function 
// it is called by main in kernel.asm
// Kernel main function to initialize the system
void kernel_main(unsigned long magic __UNUSED__, multiboot_info_t* mbi_phys) {
    // Receive BIOS information
    multiboot_info = TO_VMA_PTR(multiboot_info_t *, mbi_phys);

    // Initialize kernel serial for debugging
    init_kernel_serial();

    // Initialize kernel paging for memory management
    init_kernel_paging();

    // Initialize the Programmable Interrupt Controller (PIC)
    init_kernel_pic();

    // Initialize the Interrupt Service Routines (ISR)
    init_kernel_isr();

    // Enable ATA hard drive driver
    Ata_hd_Driver_Enable();

    // Enable FAT file system driver
    fat_hd_Driver_Enable();

    // Clear system information
    sysinfo.user_id = -1;
    sysinfo.state_count = -1;
    sysinfo.sys_mode = "";
    sysinfo.sys_id = 0;
    sysinfo.main_def = NULL;
    sysinfo.last_def = NULL;

    // Display a startup message on the screen
    Draw_String(&screen_info, screen_info.width/2-100, screen_info.height/2, 32, 32, 32, 32, screen_info.width, screen_info.height, COLOR_WHITE, COLOR_BLACK, "Addis-Os");

    /*
    void testing_process(){
        // Log debug information about the OS startup
        DEBUG("\n\n\t\t\t|\t\t\t|\n\n\n");
        DEBUG("\n\n\t\t\t| Starting Os |\n\n\n");
        DEBUG("\n\n\t\t\t|\t\t\t|\n\n\n");
        while (1) DEBUG("Process Calling Working Will")
    }
    // Create a kernel process for boot information retrieval
    // create_kernel_process_last((void*)testing_process, 1); // process testing call
    */
    create_kernel_process_last((void*)get_bootinfo, 1);

    // Jump to the first task
    do_first_task_jump();

    // Infinite loop to keep the kernel running
    while(1) { }

    // Log debug information about shutting down
    DEBUG("shuting down ...............................................................100\% \n");
    DEBUG("goodbay\n");
}








/// old code not longer used but may be useful in future

   
 /*
void get_bootinfo() {
    // Function to check and handle different system modes
    void chack_(){
        if (issame(sysinfo.sys_mode, "~GUI")) {
            // Handle GUI mode
            // code read codes from Sys/Driv/GUI/Guidriv.txt
        }
        else if (issame(sysinfo.sys_mode, "~CMD")) {
            // Handle CMD mode
            /// code read codes from Sys/Driv/GUI/Cmddriv.txt
        }
        else if (sysinfo.USER_NAME == NULL && issame(sysinfo.sys_mode, "~GUI")) {
            // Handle GUI mode with no user logged in
            int j = task_list_current->holded_info->read->bootinfo->lines += 2;
            task_list_current->holded_info->read->bootinfo->code_splited1[j-1] = "log";
            task_list_current->holded_info->read->bootinfo->code_splited1[j] = "/boot/logStart.oac";
        }
        else if (!sysinfo.started && !(issame(sysinfo.sys_mode, "~MAIN"))) {
            // Handle system startup
            int j = task_list_current->holded_info->read->bootinfo->lines += 2;
            task_list_current->holded_info->read->bootinfo->code_splited1[j-1] = "START";
            sysinfo.userpath = stradd("/user/", sysinfo.USER_NAME, 0);
            task_list_current->holded_info->read->bootinfo->code_splited1[j] = stradd(sysinfo.userpath, "/start.oac", 0);
            sysinfo.started = 1;
        }
        else {
            // Handle default system mode
            DEBUG("Choseing boot Start\n");
            int j = task_list_current->holded_info->read->bootinfo->lines +=2;
            task_list_current->holded_info->read->bootinfo->code_splited1[j-1] = "MAIN";
            task_list_current->holded_info->read->bootinfo->code_splited1[j] = "/boot/Main.oac";
            state = PROCESS_STATE_READ_TO_ONE_END;
            sysinfo.sys_mode = stradd("", "~MAIN", 0);
        }
    }

    int j, i;

    // Log debug information about reading boot paths
    DEBUG("reading bootpathes\n");

    // Check if boot information is already loaded
    if(!task_list_current->holded_info->read->bootinfo){
        // Load boot information from the specified path
        DEBUG("going to get boot path\n");
        task_list_current->holded_info->read->bootinfo = get_pathlists("/boot/paths.txt");
        task_list_current->holded_info->read->booton = 0;
        j = task_list_current->holded_info->read->bootinfo->lines;
    }
    else j = task_list_current->holded_info->read->bootinfo->lines;

    // Log debug information about reading boot code
    DEBUG("going to read boot code\n");

    i = task_list_current->holded_info->read->booton;

    // Check if there are more boot codes to read
    if (j >= i) {
        i++;

        // Log debug information about the code being read
        DEBUG(" geting code from[%d]<%s>\n", i, task_list_current->holded_info->read->bootinfo->code_splited1[i]);

        // Get the code from the specified path
        char *code = get_code(task_list_current->holded_info->read->bootinfo->code_splited1[i]);

        // Log debug information about the code retrieval
        DEBUG(" done ");

        if (code != NULL) {
            // If the code is valid, create a new kernel process for it
            DEBUG("gatting code\n");
            create_kernel_process_last((void*)Read_focused, state);
            Create_readinfo('L');
            Create_working_place();

            // Log debug information about the code preparation
            DEBUG(" going to create and prepar to fix Code(%s)....\n", code);

            if (code && !(issame(code, "") || issame(code, " "))) {
                // Update the task with the retrieved code
                task_list_last->holded_info->read->fread->reading_task = task_list_last;
                task_list_last->holded_info->read->fread->name = strdup(task_list_current->holded_info->read->bootinfo->code_splited1[i-1]);
                task_list_last->holded_info->read->fread->getas = "";
                task_list_last->holded_info->read->fread->getwhat = "";
                task_list_last->holded_info->read->fread->read_new = stradd(code," ", 0);
                task_list_last->holded_info->read->fread->rfor_id = 0;
                task_list_last->holded_info->read->fread->reading_for[0] = "READING";

                // Log debug information about the task creation
                DEBUG(" done creating(task-id[%d] by state[%d]) and fixing code %s\n", task_list_last->id, task_list_current->state, code);
            }
            else {
                // If the code is invalid, delete the last created working place
                DEBUG(" can't creating and fixing code else %s\n", code);
                Delete_lateworking_place();
                DEBUG(" done deleting \n");
            }
        }
        else DEBUG("getting code from %s filed!!\n", task_list_current->holded_info->read->bootinfo->code_splited1[i]);

        i++;
        task_list_current->holded_info->read->booton = i;

        // Log debug information about the completed boot info reading
        DEBUG("DONE Adding reading bootinfo\n");
    }
    else {
        // If no more boot codes are left, check the system mode
        DEBUG(" done reading bootinfo\n");
        chack_();
    }

    // Reload the current task to process it again
    // reload_current_task();
}*/