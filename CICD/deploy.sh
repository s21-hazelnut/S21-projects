#!/usr/bin/expect -f

# sudo su gitlab-runner
# sudo ssh-keygen
# scp .ssh/id_rsa.pub  student@172.24.116.8:/home/student/.ssh/

# scp /home/student/DO6_CICD-0/src/cat/s21_cat /home/student/DO6_CICD-0/src/grep/s21_grep gitlab-runner@172.24.116.8:/home/student/
# ssh student@172.24.116.8 "echo student | sudo -S mv ~/s21_cat ~/s21_grep /usr/local/bin/"

sshpass -p student scp src/cat/s21_cat src/grep/s21_grep student@172.24.116.8:/home/student/

sshpass -p student ssh student@172.24.116.8 "echo student | sudo -S mv ~/s21_cat ~/s21_grep /usr/local/bin/"
