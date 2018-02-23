# EE367L Lab 5 Switched LAN simulator – part 1

（Repo of lab notes: <https://github.com/duck8880/EE367L>）

​  
## Notes

  - Submission deadline: March 8, 2018. You have 4 - 5 weeks for Lab 5.
  
  - Your lab 2 reports have been graded. Please turn in both the **revised** and **original** reports on March 1, 2018.
  
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
  
    - **host_main()**
    
      - Receive and execute a command from the manager.
      
        This may create jobs, which are put in the job queue.
        
      - Examine its incoming network link and convert a packet into a “job”, then put the job in a job queue.
      
      - Take exactly one job from the job queue, and execute the job.

        This may create more jobs, which are put in the job queue.
        
      - Go to sleep for ten milliseconds.

​  

## Assignments

  - Navigate the network simulator
  
    - Transfer LabSwitch.tar.gz and extract it using `tar -xvf LabSwitch.tar.gz`.
    
    - `make` it and run `./net367`
    
    - Navigate the command: s, m, h, c, ...

  - Improvement 1: File transfers
  
    - Improve the upload so that it can transfer files of at most 1000 bytes. 
      
      Break up a large file into multiple packets. The packets are transferred and reassembled at the destination host.
      
    - Implement a file download from another host, where a file can have up to 1000 bytes.
    
      In **man.c**, create function **file_download()**, and add it into the switch statement in function **man_main()**.
    
  - Improvement 2: Switch node
  
    - Create new files **switch.c** and possibly **switch.h** to implement the switch node.
    
  - Improvement 3: Sockets as a link option
  
    - Implement sockets to connect two subnetworks.
    
    - Extend the configuration file by adding socket links in it, e.g.,    

      S 4 wiliki.eng.hawaii.edu 3000 spectra.eng.hawaii.edu 3001

