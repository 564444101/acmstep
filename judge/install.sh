  /usr/sbin/useradd -m -u 1536 judge 
 mkdir  /home/judge/run0
 mkdir  /home/judge/run1
 mkdir  /home/judge/run2
 mkdir  /home/judge/run3
cd judge_client
 make clean
 make
 cp judge_client /usr/bin
cd ../judge_daemon
 make clean
 make
 cp judge_daemon /usr/bin
