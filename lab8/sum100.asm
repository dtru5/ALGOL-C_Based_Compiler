 
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
        li $v0, 5 #read a number from input
        syscall #reading a number
        move $t1, $v0 #copy v0 to t1
 
       
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
        la $a0, str
        li $v0 4  #   print the string
        syscall # 
        nop   # 

        #print the inputted number
        move $a0, $t1
        li $v0 1 #   Print the number
        syscall

        #print the number $t1
        la $a0, str1
        li $v0 4
        syscall
        nop
        
        lw $a0, 12($sp)
        li $v0 1 #   Print the number
        syscall
        
       
        lw $ra, 4($sp)
        lw $sp , ($sp)  # put backl Ra and SP
        
        li $v0, 10 
        syscall     #  exit for MAIN only
        
  .data
        .align 0
str:
        .asciiz "The sum from 0 .. "
str1:
        .asciiz " is \n"
        
