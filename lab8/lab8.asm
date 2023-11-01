#       Name: Dominik Trujillo
#       Date: 10/31/2023
#       Lab: LAB8 MIPS with MARS simulator
#       Purpose: The purpose of this lab is to help understand how to run a MIPS simulator as well as
#       understanding the basics of MIPS instruction set and that we have the resources to
#       look up any information related to MIPS and its instruction set.

      #  this program calculates the sum of squares up to and including 100
           

 .text  # directive that we are in the code segment

main:
        subu $a0, $sp, 12 # We want 4 mem locations for main SP, RA, i , s
        sw $ra, 4($a0)  # we need to store the RA in the Activation record
        sw $sp, ($a0) # store SP
        move $sp, $a0  # adjust SP

        sw $0, 8($sp)  # store 0 into memory -- counting variable 
        sw $0, 12($sp)  # store 0 into memory -- accumulating variable

        #Ask user for input
        la $a0, str2
        li $v0 4
        syscall

        li $v0, 5 #read a number from input
        syscall #reading a number
        move $t1, $v0 #copy v0 to t1
        sw $t1, 16($sp) # store t1 value into memory -- read in variable
 
       
loop:
        lw $t6, 8($sp)  #   
        mul $t7, $t6, $t6  #  i * i
        lw $t8, 12($sp)   #  s
        addu $t9, $t8, $t7  #  s + i*i
        sw $t9, 12($sp)  #   s= s + i*i 
        lw $t6, 8($sp)   # i 
        addu $t0, $t6, 1  # i+1
        sw $t0, 8($sp)  #  i = i + 1
        ble $t0, $t1, loop  #   keep doing it for 100 times
        nop   # 
        
        #print first part of string.
        la $a0, str # Load a0 with str 
        li $v0 4  #   print the string
        syscall # system call
        nop   # no operation

        #print the inputted number
        lw $a0, 16($sp) # load a0 with read in variable stored at stack pointer plus 16
        li $v0 1 #   Print the number
        syscall # system call

        #print the number $t1
        la $a0, str1 # load a0 with str1
        li $v0 4 # print the string
        syscall # system call
        nop # no operation 
        
        lw $a0, 12($sp) # load a0 with the sum variable which is at stack pointer plus 12
        li $v0 1 #   Print the number
        syscall
        
       
        lw $ra, 4($sp) # load ra with stack pointer plus 4
        lw $sp , ($sp)  # put backl Ra and SP
        
        li $v0, 10 
        syscall     #  exit for MAIN only
        
  .data
        .align 0
str:
        .asciiz "The sum of squares from 1 to "
str1:
        .asciiz " is: "
str2:
        .asciiz "Please enter a number: "
        
