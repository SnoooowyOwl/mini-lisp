(define (leftpart x pivot)
				(  filter ( lambda ( x ) (< x pivot)  ) x)
)
(define (rightpart x pivot)
				(  filter ( lambda ( x ) (>= x pivot)  ) x)
)

(define (qsort mylist)
				(   cond  
                                                (   (or (null? mylist)  (null?  (cdr mylist)))   mylist )
						(    else
								( let (   (pivot (car mylist))  (restlist (cdr mylist))  )


									(append   (qsort  (leftpart   restlist pivot)  )
											 (list pivot)
											 (qsort  (rightpart restlist pivot)  )
									)
								)
						)
				)
)
(define x '(12 71 2 15 29 82 87 8 18 66 81 25 63 97 40 3 93 58 53 31 47))
(print (qsort x))