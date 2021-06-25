-- i
CREATE OR REPLACE TRIGGER checker
BEFORE UPDATE OF balance ON account
REFERENCING NEW AS n OLD AS o
FOR EACH ROW
BEGIN
	IF (:n.balance < 0) THEN 
		RAISE_APPLICATION_ERROR('-20124', 'Insufficient Funds : '||:n.acctno);
	ELSE 
		INSERT INTO transaction VALUES (:n.acctno, SYSDATE, :n.balance - :o.balance);
	END IF;
END;
/

DROP TRIGGER checker;
