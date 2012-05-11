;; Functions
;;
;; "The roots of Lisp" by Paul Graham (2002)

((lambda (x) (cons x '(b))) 'a)

((lambda (x y) (cons x (cdr y))) 'z '(a b c))

(defun xsubst (x y z)
               (cond ((atom z)
                      (cond ((eq z y) x)
                            ('t z)))
                      ('t (cons (xsubst y x (car z))
                                (xsubst x y (cdr z))))))
