 /usr/sbin/useradd -m -u 1538 vjudge 
 mkdir  /home/vjudge/run0
 mkdir  /home/vjudge/run1
 mkdir  /home/vjudge/run2
 mkdir  /home/vjudge/run3
cd judge_client
 make clean
 make
 cp vjudge_client /usr/bin
cd ../judge_daemon
 make clean
 make
 cp vjudge_daemon /usr/bin
