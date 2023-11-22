#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo ""

for var in -v -c -l -n -h -i -s 
do
  for var2 in -v -c -l -n -h -i -s
  do
      for var3 in -v -c -l -n -i -s
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST1="$var $var2 $var3 for s21_grep.c s21_grep.h Makefile"
          # echo "$TEST1"
          ./s21_grep $TEST1 > s21_grep1.txt
          grep $TEST1 > grep1.txt
          DIFF_RES="$(diff -s s21_grep1.txt grep1.txt)"
          if [ "$DIFF_RES" == "Files s21_grep1.txt and grep1.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep1.txt grep1.txt

          TEST2="$var $var2 $var3 for s21_grep.c"
          # echo "$TEST2"
          ./s21_grep $TEST2 > s21_grep2.txt
          grep $TEST2 > grep2.txt
          DIFF_RES="$(diff -s s21_grep2.txt grep2.txt)"
          if [ "$DIFF_RES" == "Files s21_grep2.txt and grep2.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST2"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep2.txt grep2.txt

          TEST3="$var $var2 $var3 -e ^int s21_grep.c s21_grep.h Makefile"
          # echo "$TEST3"
          ./s21_grep $TEST3 > s21_grep3.txt
          grep $TEST3 > grep3.txt
          DIFF_RES="$(diff -s s21_grep3.txt grep3.txt)"
          if [ "$DIFF_RES" == "Files s21_grep3.txt and grep3.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST3"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep3.txt grep3.txt

          TEST4="-e for s21_grep.c $var $var2 $var3"
          # echo "$TEST4"
          ./s21_grep $TEST4 > s21_grep4.txt
          grep $TEST4 > grep4.txt
          DIFF_RES="$(diff -s s21_grep4.txt grep4.txt)"
          if [ "$DIFF_RES" == "Files s21_grep4.txt and grep4.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST4"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep4.txt grep4.txt

          TEST5="s21_grep.c $var $var2 $var3 -f pattern.txt"
          # echo "$TEST5"
          ./s21_grep $TEST5 > s21_grep5.txt
          grep $TEST5 > grep5.txt
          DIFF_RES="$(diff -s s21_grep5.txt grep5.txt)"
          if [ "$DIFF_RES" == "Files s21_grep5.txt and grep5.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST5"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep5.txt grep5.txt

          TEST6=" Makefile s21_grep.c $var $var2 $var3 -f pattern.txt"
          # echo "$TEST6"
          ./s21_grep $TEST6 > s21_grep6.txt
          grep $TEST6 > grep6.txt
          DIFF_RES="$(diff -s s21_grep6.txt grep6.txt)"
          if [ "$DIFF_RES" == "Files s21_grep6.txt and grep6.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST6"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep6.txt grep6.txt

        fi
      done
  done
done

for var in -v -c -l -n -h -i -s 
do
          TEST1="$var -f pattern.txt s21_grep.h Makefile"
          # echo "$TEST1"
          ./s21_grep $TEST1 > s21_grep1.txt
          grep $TEST1 > grep1.txt
          DIFF_RES="$(diff -s s21_grep1.txt grep1.txt)"
          if [ "$DIFF_RES" == "Files s21_grep1.txt and grep1.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep1.txt grep1.txt
      done

      for var in -v -c -l -n -h -i -s 
do
          TEST1="$var -f pattern.txt s21_grep.h"
          # echo "$TEST1"
          ./s21_grep $TEST1 > s21_grep1.txt
          grep $TEST1 > grep1.txt
          DIFF_RES="$(diff -s s21_grep1.txt grep1.txt)"
          if [ "$DIFF_RES" == "Files s21_grep1.txt and grep1.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1"
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep1.txt grep1.txt
      done


echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"
