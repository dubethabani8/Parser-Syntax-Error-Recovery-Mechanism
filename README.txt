Names: Thabani Dube and Benson Gathee
NetId: tdube2 and bgathee

A2: SYNTAX ERROR RECOVERY

SOURCE CODE FILES IN THIS SUBMISSION:
    1. scan.h
    2. scan.cpp
    3. parse.cpp
    The rest are test files and compilation files if compliled already

HOW TO RUN AND TEST DIFFFERENT CASES:
        1. Complile with command [g++ scan.h scan.cpp parse.cpp]
        2. Run with command [./a.out test.txt]
        In place of test.txt, you can use any file but in this submission, I have also included a few test files:
            1. test1.txt - exactly as example given in handout
            2. test2.txt, test3.txt, ... , test10.txt
        On the console, error messages will be printed or correct output will be show in Linear parenthesized form
        

IMPLEMENTATION DETAILS:
    1. C to C++ translation
    -We made changes to allow for compilation with g++ including no printfs and other changes.

    2. Extending the language
    -We extended the language approrpiately to include while, if and associated conditionals as required.

    3. Syntax Error Recovery
    -We implememted exception based sytax error recovery based on the lecture.
    -On token mismatch, we insert what we expected and print an error message.
    -On null predictions, we throw syntax error exceptions.
    -exceptions are caught by handlers in the routines.
    -the exception handlers loop while input token is not EOF or in the follow set,
     and gets next token until condition is met, essentially deleting to what we 
     hope is the end for simplicity.

     4.  Linear parenthesized form
     -For correct output you can expect output in Linear parenthesized forn
     -For example:
     sample correct input in handout produces the following:

     (program[ (read "n") ("cp" := (literal "2")) (while ((id "n") > (literal "0"))
     [ ("found" := (literal "0")) ("cf1" := (literal "2")) ("cf1s" := (id "cf1") * (id "cf1"))
     (while ((id "cf1s") <= (id "cp")) [ ("cf2" := (literal "2")) ("pr" := (id "cf1") * (id "cf2"))
     (while ((id "pr") <= (id "cp")) [ (if ((id "pr") = (id "cp")) [ ("found" := (literal "1")) ])
     ("cf2" := (id "cf2") + (literal "1")) ("pr" := (id "cf1") * (id "cf2")) ]) ("cf1" := (id "cf1")
     + (literal "1")) ("cf1s" := (id "cf1") * (id "cf1")) ]) (if ((id "found") = (literal "0"))
     [ ( write (id "cp")) ("n" := (id "n") - (literal "1")) ]) ("cp" := (id "cp") + (literal "1")) ]) ])


Thank you!



