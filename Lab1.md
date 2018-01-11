# EE367L Lab 1 Introduction
## Note

  The lab is individual work.

  You can use either your laptops or the PCs in the lab. We have 22 PCs in the lab.



## TA

  Jianqiu "Chris" Cao (jianqiuc@hawaii.edu)
## Lab time and location
  Thursdays at 4:30-7:30p, POST 214
## Self Introduction
  Name, Major, Year (junior or senior)  
  Send me an email with your introduction and a face picture attached



## Part A: Basic Commands

- Login to Wiliki account using **PuTTY**  
  `Host Name: wiliki.eng.hawaii.edu`, `Port: 22`

- Create directory **EE367**. 

- Use **FileZilla client** (not FileZilla server) to transfer **Lab1.tar.gz** to **EE367**  
  `Host: wiliki.eng.hawaii.edu`, `Port: 22`

- Untar the file  
  `tar -xf Lab1.tar.gz`

- Take the steps 5 - 10 in the handout. 

  - Useful commands in Unix:

  ​      `mkdir`, `cd`, `pwd`, `ls`, `cat`, `vi`, `gcc`

  - Useful command in vi:

    `:i`, `:a`, `x`, `dd`, `:q`, `:wq`, `:q!`




## Part B: Shell Scripts

- Transfer **Lab1B.tar** to Wiliki account and untar it

- Make **sum** and run it

  `make`

  `./sum Data/datafile-0`

- Run shell scripts **scriptarg** and **scriptlist**

  `./scriptarg`

  `./scriptlist`

- Create a shell script **scriptlab1** which uses **sum** to process each data file in **Data** and outputs results into file **lab1b-results**. Refer to command lines in **scriptlist**.

  - Output redirection: `> filename`(overwrite) or `>> filename`(append)

- Change mode of  **scriptlab1** to 775 and run it

  `chmod 775 scriptlab1`

  `./scriptlab1`

- Demonstrate it to the TA. Tar **scriptlab1** and gzip it. Then upload it into laulima:

  `tar -czvf scriptlab.tar.gz scriptlab1`

  ​





