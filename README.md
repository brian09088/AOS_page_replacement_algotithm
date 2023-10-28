# AOS_page_replacement_algotithm
## Specification:
1. Reference string: 1~1,000
2. Number of memory references: At least 200,000 times
3. Number of frames in the physical memory: 20, 40, 60, 80, 100
4. Three test reference strings:
(1) Random: Arbitrarily pick [1, 20] continuous numbers for each reference.
(2) Locality: Simulate function calls. Each function call may refer a subset of 1/30~1/20
string (the length of string can be random).
(3) Your own reference string: You need to explain why you choose the reference string
in the report.
5. You can use both reference and dirty bits.
## Requirements:
1. You have to implement THREE algorithms for comparison:
(1) FIFO algorithm
(2) ARB algorithm (8-bit information)
(3) Enhanced second-chance algorithm
2. In addition, you should develop your own algorithm (not in the textbook). Your algorithm
is expected to at least win the FIFO one (in terms of the page-fault rate or cost), where the
cost is defined by the number of interrupts required and the number of pages needed to be
written back to the disk. Recall that every time when you invoke the OS to do something,
an interrupt is always necessary.
3. For each algorithm and reference string, your report should present the following three
figures:
(1) The relationship between page faults and the number of frames.
(2) The relationship between the number of interrupts and the number of frames.
(3) The relationship between the number of disk writes (in pages) and the number of
frames.
Furthermore, your report should give some discussions about the behaviors of these
algorithms.
4. You need to demonstrate your program to TA (no demonstration, no points!) and submit
your report in WORD.
