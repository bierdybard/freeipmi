;;; bc-serial-conf-section.scm: BMC configurator Serial Conf
;;;                             section procedures
;;; authors: Balamurugan <bala.a@californiadigital.com>

;;; This program is free software; you can redistribute it and/or
;;; modify it under the terms of the GNU General Public License as
;;; published by the Free Software Foundation; either version 2, or (at
;;; your option) any later version.
;;; 
;;; This program is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;;; General Public License for more details.
;;; 
;;; You should have received a copy of the GNU General Public License
;;; along with this program; if not, write to the Free Software
;;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA

(define (commit-enable-basic-mode section-name enable-basic-mode)
  (if (list? enable-basic-mode)
      #t 
      (fi-set-bmc-serial-conf-connection-mode enable-basic-mode 0 0 0)))

(define (checkout-enable-basic-mode section-name)
  (let ((param-list (fi-get-bmc-serial-conf-connection-mode)))
    (if (list? param-list) (list (car param-list)) #f)))

(define (commit-enable-ppp-mode section-name enable-ppp-mode)
  (if (list? enable-ppp-mode)
      #t 
      (fi-set-bmc-serial-conf-connection-mode 0 enable-ppp-mode 0 0)))

(define (checkout-enable-ppp-mode section-name)
  (let ((param-list (fi-get-bmc-serial-conf-connection-mode)))
    (if (list? param-list) (list (cadr param-list)) #f)))

(define (commit-enable-terminal-mode section-name enable-terminal-mode)
  (if (list? enable-terminal-mode)
      #t 
      (fi-set-bmc-serial-conf-connection-mode 0 0 enable-terminal-mode 0)))
  
(define (checkout-enable-terminal-mode section-name) 
  (let ((param-list (fi-get-bmc-serial-conf-connection-mode)))
    (if (list? param-list) (list (caddr param-list)) #f)))

(define (commit-connect-mode section-name connect-mode)
  (if (list? connect-mode)
      #t 
      (fi-set-bmc-serial-conf-connection-mode 0 0 0 connect-mode)))

(define (checkout-connect-mode section-name) 
  (let ((param-list (fi-get-bmc-serial-conf-connection-mode))) 
    (if (list? param-list) (list (cadddr param-list)) #f)))

(define (commit-page-blackout-interval section-name page-blackout-interval)
  (if (list? page-blackout-interval)
      #t 
      (fi-set-bmc-serial-conf-page-blackout-interval page-blackout-interval)))

(define (checkout-page-blackout-interval section-name) 
  (fi-get-bmc-serial-conf-page-blackout-interval)) 

(define (commit-call-retry-interval section-name call-retry-interval)
  (if (list? call-retry-interval)
      #t 
      (fi-set-bmc-serial-conf-call-retry-interval call-retry-interval)))

(define (checkout-call-retry-interval section-name) 
  (fi-get-bmc-serial-conf-call-retry-interval)) 

(define (commit-enable-dtr-hangup section-name enable-dtr-hangup)
  (if (list? enable-dtr-hangup)
      #t 
      (fi-set-bmc-serial-conf-ipmi-messaging-comm-settings enable-dtr-hangup #f #f)))

(define (checkout-enable-dtr-hangup section-name) 
  (let ((param-list (fi-get-bmc-serial-conf-ipmi-messaging-comm-settings))) 
    (if (list? param-list) (list (car param-list)) #f)))

(define (commit-flow-control section-name flow-control)
  (if (list? flow-control)
      #t 
      (fi-set-bmc-serial-conf-ipmi-messaging-comm-settings 0 flow-control #f)))

(define (checkout-flow-control section-name) 
  (let ((param-list (fi-get-bmc-serial-conf-ipmi-messaging-comm-settings))) 
    (if (list? param-list) (list (cadr param-list)) #f)))

(define (commit-serial-bit-rate section-name serial-bit-rate)
  (if (list? serial-bit-rate)
      #t 
      (fi-set-bmc-serial-conf-ipmi-messaging-comm-settings 0 #f serial-bit-rate)))

(define (checkout-serial-bit-rate section-name) 
  (let ((param-list (fi-get-bmc-serial-conf-ipmi-messaging-comm-settings))) 
    (if (list? param-list) (list (caddr param-list)) #f)))

(define serial-conf-keys-validator 
  '(
    ;; You can add more in the form of 
    ;; (KEYSTRING 
    ;;  VALIDATION-PROC 
    ;;  CONVERSION-PROC 
    ;;  BMC-COMMIT-PROC 
    ;;  BMC-CHECKOUT-PROC 
    ;;  VALUE-CONVERSION-PROC 
    ;;  DIFF-PROC 
    ;;  DOC-STRING)
    ("enable_basic_mode" 
     valid-boolean? 
     get-boolean 
     commit-enable-basic-mode 
     checkout-enable-basic-mode 
     get-boolean-string 
     same-string-ci?
     "Possible values: Yes/No")
    ("enable_ppp_mode" 
     valid-boolean? 
     get-boolean 
     commit-enable-ppp-mode 
     checkout-enable-ppp-mode 
     get-boolean-string 
     same-string-ci?
     "Possible values: Yes/No")
    ("enable_terminal_mode" 
     valid-boolean? 
     get-boolean 
     commit-enable-terminal-mode 
     checkout-enable-terminal-mode 
     get-boolean-string 
     same-string-ci?
     "Possible values: Yes/No")
    ("connect_mode" 
     valid-connect-mode? 
     get-connect-mode 
     commit-connect-mode 
     checkout-connect-mode 
     get-connect-mode-value-string 
     same-string-ci?
     "Possible Values: Modem_Connect/Direct_Connect")
    ("page_blackout_interval" 
     valid-integer? 
     get-integer 
     commit-page-blackout-interval 
     checkout-page-blackout-interval 
     any->string
     same-string-ci?
     "Give valid number")
    ("call_retry_time" 
     valid-integer? 
     get-integer 
     commit-call-retry-interval 
     checkout-call-retry-interval 
     any->string
     same-string-ci?
     "Give valid number")
    ("enable_dtr_hangup" 
     valid-boolean? 
     get-boolean 
     commit-enable-dtr-hangup 
     checkout-enable-dtr-hangup 
     get-boolean-string
     same-string-ci?
     "Possible values: Yes/No")
    ("flow_control" 
     valid-flow-control? 
     get-flow-control 
     commit-flow-control 
     checkout-flow-control 
     get-flow-control-value-string
     same-string-ci?
     "Possible values: No_Flow_Control/RTS_CTS/XON_XOFF")
    ("bit_rate" 
     valid-serial-bit-rate? 
     get-serial-bit-rate 
     commit-serial-bit-rate 
     checkout-serial-bit-rate 
     get-serial-bit-rate-value-string
     same-string-ci?
     "Possible values: 9600/19200/38400/57600/115200")
    ;; You can add more in the form of 
    ;; (KEYSTRING 
    ;;  VALIDATION-PROC 
    ;;  CONVERSION-PROC 
    ;;  BMC-COMMIT-PROC 
    ;;  BMC-CHECKOUT-PROC 
    ;;  VALUE-CONVERSION-PROC 
    ;;  DIFF-PROC 
    ;;  DOC-STRING)
    ))
