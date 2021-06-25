CREATE TABLE patient (
	pat_id NUMBER PRIMARY KEY,  
	pname VARCHAR(32) NOT NULL,  
	dob DATE,  
	sex CHAR(1)
);

CREATE TABLE doctor (
    doc_id NUMBER PRIMARY KEY,  
    dname VARCHAR(32),  
    specialization VARCHAR(32), 
    unit NUMBER
);

CREATE TABLE opd_schedule (
    doc_id NUMBER, 
    sdate DATE NOT NULL, 
    stime TIMESTAMP NOT NULL, 
    fees NUMBER NOT NULL,  
    FOREIGN KEY (doc_id) REFERENCES doctor(doc_id) ON DELETE CASCADE,
    PRIMARY KEY (doc_id, sdate)
);

CREATE TABLE appointment (
    a_no NUMBER PRIMARY KEY, 
    pat_id NUMBER, 
    doc_id NUMBER, 
    adate DATE, 
    FOREIGN KEY (pat_id) REFERENCES patient(pat_id) ON DELETE CASCADE, 
    FOREIGN KEY (doc_id) REFERENCES doctor(doc_id) ON DELETE CASCADE
);

CREATE TABLE opd_payments (
    a_no NUMBER, 
    pat_id NUMBER, 
    FOREIGN KEY (pat_id) REFERENCES patient(pat_id) ON DELETE CASCADE, 
    amt NUMBER, 
    p_date DATE
);

INSERT INTO patient values(1, 'John', TO_DATE('19991230', 'YYYYMMDD'), 'M');
INSERT INTO patient values(2, 'Priya', TO_DATE('19591230', 'YYYYMMDD'), 'F');
INSERT INTO patient values(3, 'Aneeta', TO_DATE('19981230', 'YYYYMMDD'), 'F');
INSERT INTO patient values(4, 'Jack', TO_DATE('19601230', 'YYYYMMDD'), 'M');
INSERT INTO patient values(5, 'Bert', TO_DATE('19701230', 'YYYYMMDD'), 'M');
INSERT INTO patient values(6, 'Paul', TO_DATE('20031230', 'YYYYMMDD'), 'M');
INSERT INTO patient values(7, 'Parul', TO_DATE('19951230', 'YYYYMMDD'), 'F');
INSERT INTO patient values(8, 'Aman', TO_DATE('19691230', 'YYYYMMDD'), 'M');
INSERT INTO patient values(9, 'Steve', TO_DATE('19401230', 'YYYYMMDD'), 'M');
INSERT INTO patient values(10, 'Natasha', TO_DATE('19991230', 'YYYYMMDD'), 'F');
INSERT INTO patient values(11, 'Riya', TO_DATE('19801230', 'YYYYMMDD'), 'F');
INSERT INTO patient values(12, 'Aman', TO_DATE('19851230', 'YYYYMMDD'), 'M');
INSERT INTO patient values(13, 'Samarth', TO_DATE('19961230', 'YYYYMMDD'), 'M');

INSERT INTO doctor values(1, 'Jill', 'Ortho', 1);
INSERT INTO doctor values(2, 'Joe', 'Ortho', 1);
INSERT INTO doctor values(3, 'Nikhil', 'Surgery', 2);
INSERT INTO doctor values(4, 'April', 'Dentist', 2);
INSERT INTO doctor values(5, 'Ana', 'Gastro', 3);

INSERT INTO opd_schedule values(1, TO_DATE('20200130', 'YYYYMMDD'), TO_DATE('10:30 am', 'hh:mi am'), 500);
INSERT INTO opd_schedule values(1, TO_DATE('20200202', 'YYYYMMDD'), TO_DATE('09:30 am', 'hh:mi am'), 500);
INSERT INTO opd_schedule values(1, TO_DATE('20200204', 'YYYYMMDD'), TO_DATE('10:30 am', 'hh:mi am'), 500);
INSERT INTO opd_schedule values(1, TO_DATE('20200203', 'YYYYMMDD'), TO_DATE('12:30 am', 'hh:mi am'), 500);
INSERT INTO opd_schedule values(1, TO_DATE('20200131', 'YYYYMMDD'), TO_DATE('11:00 am', 'hh:mi am'), 500);
INSERT INTO opd_schedule values(2, TO_DATE('20200131', 'YYYYMMDD'), TO_DATE('10:30 am', 'hh:mi am'), 600);
INSERT INTO opd_schedule values(2, TO_DATE('20200127', 'YYYYMMDD'), TO_DATE('09:30 am', 'hh:mi am'), 600);
INSERT INTO opd_schedule values(3, TO_DATE('20200125', 'YYYYMMDD'), TO_DATE('03:30 pm', 'hh:mi pm'), 700);
INSERT INTO opd_schedule values(3, TO_DATE('20200124', 'YYYYMMDD'), TO_DATE('03:00 pm', 'hh:mi pm'), 700);
INSERT INTO opd_schedule values(3, TO_DATE('20200131', 'YYYYMMDD'), TO_DATE('04:00 pm', 'hh:mi pm'), 700);
INSERT INTO opd_schedule values(3, TO_DATE('20200201', 'YYYYMMDD'), TO_DATE('08:30 pm', 'hh:mi pm'), 700);
INSERT INTO opd_schedule values(4, TO_DATE('20200125', 'YYYYMMDD'), TO_DATE('08:40 pm', 'hh:mi pm'), 400);
INSERT INTO opd_schedule values(4, TO_DATE('20200131', 'YYYYMMDD'), TO_DATE('09:00 pm', 'hh:mi pm'), 400);
INSERT INTO opd_schedule values(4, TO_DATE('20200130', 'YYYYMMDD'), TO_DATE('05:00 pm', 'hh:mi pm'), 400);
INSERT INTO opd_schedule values(5, TO_DATE('20200201', 'YYYYMMDD'), TO_DATE('05:00 pm', 'hh:mi pm'), 500);
INSERT INTO opd_schedule values(5, TO_DATE('20200130', 'YYYYMMDD'), TO_DATE('05:00 pm', 'hh:mi pm'), 500);

-- 1
CREATE OR REPLACE TRIGGER payments
BEFORE INSERT ON appointment
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE 
    pvisit INT;
    nfees opd_payments.amt%TYPE;
    psex patient.sex%TYPE;
    pdob patient.dob%TYPE;
BEGIN
    SELECT COUNT(*) INTO pvisit FROM appointment WHERE pat_id = :n.pat_id AND doc_id = :n.doc_id AND :n.adate - adate < 7;
    SELECT fees INTO nfees FROM opd_schedule WHERE doc_id = :n.doc_id AND sdate = :n.adate;
    SELECT sex, dob INTO psex, pdob FROM patient WHERE pat_id = :n.pat_id;
    IF pvisit > 0 THEN
        IF SYSDATE - pdob >= 60 AND psex = 'F' THEN nfees := nfees / 2;
        ELSE nfees := 0;
        END IF;
    END IF;
    INSERT INTO opd_payments VALUES (:n.a_no, :n.pat_id, nfees, :n.adate);
END;
/

INSERT INTO appointment values(1, 1, 1, TO_DATE('20200130', 'YYYYMMDD'));
INSERT INTO appointment values(2, 1, 1, TO_DATE('20200203', 'YYYYMMDD'));
INSERT INTO appointment values(3, 2, 2, TO_DATE('20200127', 'YYYYMMDD'));
INSERT INTO appointment values(4, 3, 2, TO_DATE('20200127', 'YYYYMMDD'));
INSERT INTO appointment values(5, 4, 3, TO_DATE('20200125', 'YYYYMMDD'));
INSERT INTO appointment values(6, 5, 3, TO_DATE('20200124', 'YYYYMMDD'));
INSERT INTO appointment values(7, 6, 4, TO_DATE('20200125', 'YYYYMMDD'));
INSERT INTO appointment values(8, 7, 4, TO_DATE('20200125', 'YYYYMMDD'));
INSERT INTO appointment values(9, 8, 4, TO_DATE('20200125', 'YYYYMMDD'));
INSERT INTO appointment values(10, 9, 4, TO_DATE('20200125', 'YYYYMMDD'));
INSERT INTO appointment values(11, 10, 5, TO_DATE('20200201', 'YYYYMMDD'));
INSERT INTO appointment values(12, 11, 4, TO_DATE('20200130', 'YYYYMMDD'));
INSERT INTO appointment values(13, 12, 5, TO_DATE('20200130', 'YYYYMMDD'));
INSERT INTO appointment values(14, 13, 1, TO_DATE('20200131', 'YYYYMMDD'));
INSERT INTO appointment values(15, 8, 1, TO_DATE('20200131', 'YYYYMMDD'));
INSERT INTO appointment values(16, 6, 4, TO_DATE('20200131', 'YYYYMMDD'));
INSERT INTO appointment values(17, 3, 1, TO_DATE('20200130', 'YYYYMMDD'));
INSERT INTO appointment values(18, 2, 5, TO_DATE('20200201', 'YYYYMMDD'));
INSERT INTO appointment values(19, 1, 5, TO_DATE('20200130', 'YYYYMMDD'));
INSERT INTO appointment values(20, 7, 3, TO_DATE('20200131', 'YYYYMMDD'));
INSERT INTO appointment values(21, 7, 2, TO_DATE('20200131', 'YYYYMMDD'));
INSERT INTO appointment values(22, 8, 3, TO_DATE('20200131', 'YYYYMMDD'));
INSERT INTO appointment values(23, 1, 1, TO_DATE('20200202', 'YYYYMMDD'));
INSERT INTO appointment values(24, 1, 1, TO_DATE('20200204', 'YYYYMMDD'));

ALTER TABLE opd_payments 
    ADD CONSTRAINT app_id_fk FOREIGN KEY (a_no) REFERENCES appointment(a_no) ON DELETE CASCADE;

SELECT * FROM opd_payments;

CREATE OR REPLACE PROCEDURE findAppointments(pid IN NUMBER, cur OUT SYS_REFCURSOR) AS
BEGIN
    OPEN cur FOR SELECT dname, adate FROM appointment NATURAL JOIN doctor WHERE pat_id = pid;
END;
/
