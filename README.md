# Purpose of the software

## Personal Reasoning
Bro I'm just tryna get better at C before CSSE2310 makes me go question my course load 
this sem. 

## Practical Reasoning
Consider the following operations:
Take any integer postive `n`, where `n` >= 1

If `n` is even, then n = n / 2
If `n` is odd, then n = 3n + 1

The Collatz conjecture is: This process will eventually reach the number 1, 
regardless of which positive integer is chosen initially. That is, for each 
`n`, there is some `i` with `a_i = 1`

Reference and for further information: [Collatz Conjecture](https://en.wikipedia.org/wiki/Collatz_conjecture)

This server has a centralized database to hold whether or not a number has been tested
Clients can test out numbers for fun to verify if it is valid or not
The server computes the request and caches if the number has already been tested for future cases

# Running the program

```bash
    make compile
    make run
```

# Improvements ahead 

0. Gotta debug for multiple clients connecting and JSUT ALWAYS GOTTA DEBUG (IT FEELS ENDLESS)
1. Threading to allow for simulateous computation of numbers and accepting requests.
2. I want to make the limit of the testable numbers not limited by sizeof(uint64_t) (8 Bytes =
n <= 18,446,744,073,709,551,615). This will require a lot of refactoring and prob stealing 
code from the Big NUM GNU Library (GNU THE GOAT except for the whole emacs thing).
3. Fail safety for malicious messages trying to create errors in the program or possibly
stealing data (this would be very cool and is prob possible bc i dont care about safety).

***Order of improvements above is prob how I would like to go through with it***
