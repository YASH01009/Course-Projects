-- ii
-- method 1: program approach
DECLARE
	my_acctno INT;
	my_balance INT;
	ex_balance INT;
	no_trans INT;
BEGIN
	my_acctno := &my_acctno;
	SELECT balance INTO ex_balance FROM account WHERE acctno = my_acctno;
	my_balance := &my_balance + ex_balance;
	SELECT COUNT(*) INTO no_trans FROM transaction WHERE acctno = my_acctno AND amount < 0 AND TO_DATE(tdate, 'DD/MM/YYYY') = TO_DATE(SYSDATE, 'DD/MM/YYYY');
	IF(no_trans >= 3) THEN 
		UPDATE account SET balance = my_balance - 50 WHERE acctno = my_acctno;
	ELSE 
		UPDATE account SET balance = my_balance WHERE acctno = my_acctno;
	END IF;
END;
/

-- method 2: trigger approach
-- self contained-trigger
-- trigger in queries_q3_01.sql has to be disabled before this is used
CREATE OR REPLACE TRIGGER check_t
BEFORE UPDATE OF balance ON account
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE 
	tno INT;
BEGIN
	IF (:n.balance > :o.balance) THEN 
		INSERT INTO transaction VALUES (:n.acctno, SYSDATE, :n.balance - :o.balance);
	ELSE 
		SELECT COUNT(*) INTO tno FROM transaction WHERE acctno = :n.acctno AND amount < 0 AND TO_DATE(tdate, 'DD/MM/YYYY') = TO_DATE(SYSDATE, 'DD/MM/YYYY');
		IF (tno >= 3) THEN
			IF (:n.balance >= 50) THEN 
				:n.balance := :n.balance - 50;
				INSERT INTO transaction VALUES (:n.acctno, SYSDATE, :n.balance - :o.balance);
			ELSE 
				RAISE_APPLICATION_ERROR('-20124', 'Insufficient Funds : '||:n.acctno);
			END IF;
		ELSE 
			IF (:n.balance >= 0) THEN 
				INSERT INTO transaction VALUES (:n.acctno, SYSDATE, :n.balance - :o.balance);
			ELSE 
				RAISE_APPLICATION_ERROR('-20124', 'Insufficient Funds : '||:n.acctno);
			END IF;
		END IF;
	END IF;
END;
/

DROP TRIGGER check_t;