;; Seven primitive operators
;;
;; "The roots of Lisp" by Paul Graham (2002)

(quote a)

(atom 'a)

(atom '(a b c))

(atom (atom 'a))

(atom '(atom 'a))

(eq 'a 'a)

(eq 'a 'b)

(car '(a b c))

(cdr '(a b c))

(cons 'a '(b c))

(cons 'a (cons 'b (cons 'c '())))

(car (cons 'a '(b c)))

(cdr (cons 'a '(b c)))

(cond ((eq 'a 'b) 'first)
      ((atom 'a) 'second))
