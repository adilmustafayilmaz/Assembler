again:
       addi $s1, $s2, 4
       sll $s1, $s2, 4
       add $s1, $s2, $s3
       j again
hop:
       add $s1, $s2, $s3
       add $s1, $s2, $s3
       j hop
test:
      add $s1, $s2, $s3
      add $s1, $s2, $s3
      add $s1, $s2, $s3
      add $s2, $s2, $s1
      j test