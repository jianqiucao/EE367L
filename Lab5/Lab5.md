# EE367L Lab 5 Switched LAN simulator – part 1

（Repo of lab notes: <https://github.com/duck8880/EE367L>）

​  
## Notes

  - Submission deadline: **March 22**, 2018. Show the TA your program on Lab.
  
    You can also show your program later until the end of the semester with a 10% penalty.
  
  - Lab 6 report (Contemporary Issues and Engineering Impacts) is due on **March 15**, 2018
  
  - Each one in the group can work independently toward one improvement among the three.
  
  - You can finally merge your files using GitHub and test the intergrated program then.


​  
## Files

  - **main.c**: the main routine.
  
    - **main()**:
    
      - Read network configuration file, create the lists of nodes and links and implement them.
    
      - Create the network nodes as children processes
    
      - Start the main routine for the manager
    
  - **man.c**: the manager software including the main routine of the manager.
  
    - **man_main()**: Parse and execute the command enter by the user.
    
    - **file_upload()**: Send the command 'u' (upload) to a host, which will then send a file to another host.
    
  - **host.c**: the host node software including the main routine for the host.
  
    - **host_main()**: has a infinite loop, where it does the following in each pass
    
      1. Receive and execute a command from the manager.
      
         This may create jobs, which are put in the job queue.
        
      2. Examine its incoming network link and convert a packet into a “job”, then put the job in a job queue.
      
      3. Take exactly one job from the job queue, and execute the job.

         This may create more jobs, which are put in the job queue.
        
      4. Go to sleep for ten milliseconds.
  
  - **host.h**: defines data types
  
    - The enum type **host_job_type** is defined here. We may need to define more types of jobs in it.
    
  - **switch.c**: We will create the file to implement the switch node, which is similar to a host node but has no connection to the manager.
  
    - **host_main()**: has a infinite loop, where it does the following in each pass
    
      1. Examine its incoming network link and convert a packet into a “job”, then put the job in a job queue.
      
      2. Take exactly one job from the job queue, and execute the job.

         This may create more jobs, which are put in the job queue.
        
      3. Go to sleep for ten milliseconds.
    

​  

## Assignments

  - Navigate the network simulator
  
    - Transfer LabSwitch.tar.gz and extract it using `tar -xvf LabSwitch.tar.gz`.
    
    - `make` it and run `./net367`
    
    - Navigate the command: s, m, h, c, ...

  - Improvement 1: File transfers
  
    - Improve the upload so that it can transfer files of at most 1000 bytes. 
      
      Hints:
      
      - Break up a large file into multiple packets. The packets are transferred and reassembled at the destination host.
      
      - When the source host executing the job **JOB_FILE_UPLOAD_SEND** (line 493 in host.c), insert more intermediate packets **PKT_FILE_UPLOAD_IMD** between the start and the end packets and create jobs **JOB_SEND_PKT_ALL_PORTS** to send them.
      
      - When the destination host got the packet, add a case **PKT_FILE_UPLOAD_IMD** in which we create a job **JOB_FILE_UPLOAD_RECV_IMD** to deal with the intermediate packet (line 367 in host.c). 
      
      - Add a case **JOB_FILE_UPLOAD_RECV_IMD** (around line 595 in host.c) to write the file contents into disk.
      
    - Implement a file download from another host, where a file can have up to 1000 bytes.
    
      Hints:
      
      - In **man.c**, create function **file_download()** to send a command '**d**' to the host, and add it into the switch statement in function **man_main()** (line 239 in man.c).
      
      - In **host.c**, add a case '**d**' (line 293 in host.c), in which create a job **JOB_FILE_DOWNLOAD_SEND**. Add JOB_FILE_DOWNLOAD_SEND to the enum **host_job_type** in **host.h**

      - In the job **JOB_FILE_DOWNLOAD_SEND**, the file receiving host a request to the file sending host, who check the existance of the files. If the file exists, using the mechanizm similiar to uploading to download the file.
    
  - Improvement 2: Switch node
  
    Hints:
    
    - Create new files **switch.c** and possibly **switch.h** to implement the switch node.
      The structure of the switch.c is similar to the host.c, which contains a infinite loop to deal with the jobs, except that the switch has no connection to the manager.
      
    - Add switch node and more host nodes into the configuration file.
    
  - Improvement 3: Sockets as a link option

    - Implement sockets to connect two subnetworks.

    - Extend the configuration file by adding socket links in it, e.g.,    

      S 4 wiliki.eng.hawaii.edu 3000 spectra.eng.hawaii.edu 3001

    Hints:

    - Refer to Lab 3 slides and handout for how to implement sockets. Also I have uploaded my source files here: 
      <https://github.com/duck8880/EE367L/tree/master/Lab3>

    - The nodes at each end of the link should be both "client" and "server". To establish a Symmetrical link, implement two sockets by writing the information in configuration files, e.g., (The topology is in Figure 6 in the handout): 

      In the configuration file for Machine 1, we write:  
      `S 4 wiliki.eng.hawaii.edu 3000 spectra.eng.hawaii.edu 3001`  

      which means node 4 as a server, have the domain name "wiliki.eng.hawaii.edu" and the port number "3000", and as a client connects to the server at "spectra.eng.hawaii.edu: 3001".

      In the configuration file for Machine 2, we write:  
      `S 5 spectra.eng.hawaii.edu 3001 wiliki.eng.hawaii.edu 3000`

    - Use the port number assigned to you in the Lab 3 handout. Assign different IDs to nodes even if they are in different machine.




