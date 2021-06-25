SET SERVEROUTPUT ON;

-- i
CREATE OR REPLACE TRIGGER check_t
BEFORE INSERT ON registration
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE 
	tot_course INT;
	tot_credit course.credit%TYPE;
	cur_credit course.credit%TYPE;
	cours_type course.course_type%TYPE;
	preq_count INT;
BEGIN
	SELECT COUNT(*) INTO tot_course FROM registration WHERE roll = :n.roll AND semester_no = :n.semester_no;
	IF (tot_course < 6) THEN
		SELECT NVL(SUM(credit),0) INTO tot_credit FROM ((SELECT courseno FROM registration WHERE roll = :n.roll 
		AND semester_no = :n.semester_no) NATURAL JOIN (SELECT credit, courseno FROM course));
		SELECT credit, course_type INTO cur_credit, cours_type FROM course WHERE courseno = :n.courseno;
		IF (tot_credit + cur_credit <= 24) THEN
			IF (cours_type = 'T') THEN
				SELECT COUNT(*) INTO preq_count FROM (SELECT prcourseno FROM prerequisite WHERE courseno = :n.courseno 
				MINUS SELECT courseno FROM grade WHERE roll = :n.roll AND igrade >= 6);
				IF (preq_count > 0) THEN
					RAISE_APPLICATION_ERROR('-20124', 'Registration denied : grade in all prerequisite courses should be atleast 6');
				END IF;
			END IF;
			IF (cours_type = 'E') THEN
				SELECT COUNT(*) INTO preq_count FROM (SELECT prcourseno FROM prerequisite WHERE courseno = :n.courseno 
				MINUS SELECT courseno FROM grade WHERE roll = :n.roll);
				IF (preq_count > 0) THEN
					RAISE_APPLICATION_ERROR('-20124', 'Registration denied : not completed in all prerequisite courses required');
				END IF;
			END IF;
		ELSE
			RAISE_APPLICATION_ERROR('-20124', 'Registration denied : total credits in the semester can not exceed 24');
		END IF;
	ELSE
		RAISE_APPLICATION_ERROR('-20124', 'Registration denied : total courses in the semester can not exceed 6');
	END IF;
END;
/

-- adding more items for testing
INSERT ALL
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (11, 'Infrastructure Management', 'CE', 1, 'T', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (20, 'Reinforced Concrete', 'CE', 2, 'T', 1)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (12, 'Construction Management', 'CE', 2, 'T', 9)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (13, 'Planning Management', 'CE', 2, 'E', 2)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (14, 'Traffic Engineering', 'CE', 2, 'T', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (15, 'Materials Engineering', 'CE', 2, 'T', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (16, 'Solid Mechanics', 'CE', 2, 'T', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (17, 'Surveying Practice', 'CE', 2, 'T', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (18, 'Structural Analysis', 'CE', 2, 'T', 2)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (19, 'Environmental Science', 'CE', 2, 'T', 2)
SELECT * FROM dual;

INSERT INTO student VALUES (8, 'Ravichandran Ashwin', 'CE', 2020, 'ra12');

INSERT INTO prerequisite VALUES (19, 11);
INSERT INTO prerequisite VALUES (13, 20);

INSERT INTO registration (roll, courseno, semester_no) VALUES (8, 14, 2);
INSERT INTO registration (roll, courseno, semester_no) VALUES (8, 15, 2);
INSERT INTO registration (roll, courseno, semester_no) VALUES (8, 16, 2);
INSERT INTO registration (roll, courseno, semester_no) VALUES (8, 17, 2);
-- credits: 16, courses: 4

-- demo: credits <= 24
INSERT INTO registration VALUES (8, 12, 2);

-- demo: prerequites for a core-theory course
-- fails
INSERT INTO registration VALUES (8, 19, 2); 
-- enrolling and grading (<6) for the prerequisite
INSERT INTO registration VALUES (8, 11, 1); 
INSERT INTO grade VALUES (8, 11, 5);		
-- fails
INSERT INTO registration VALUES (8, 19, 2); 
-- upgrading the grade in the prerequisite
UPDATE grade SET igrade = 9 WHERE roll = 8 AND courseno =  11;
-- suceeds
INSERT INTO registration VALUES (8, 19, 2); 
-- credits: 18, courses: 5

-- demo: prerequites for an elective course
-- fails
INSERT INTO registration VALUES (8, 13, 2); 
-- enrolling and grading for the prerequisite
INSERT INTO registration VALUES (8, 20, 1); 
INSERT INTO grade VALUES (8, 20, 5);
-- suceeds
INSERT INTO registration VALUES (8, 13, 2);
-- credits: 20, courses: 6

-- demo: courses <= 6
INSERT INTO registration (roll, courseno, semester_no) VALUES (8, 18, 2);

-- ii
CREATE TABLE attendance_warning (
	roll INT,
	adate DATE,
	courseno INT,
	PRIMARY KEY (roll, adate, courseno)
);

CREATE TABLE deregistration (
	roll INT,
	adate DATE,
	courseno INT,
	PRIMARY KEY (roll, adate, courseno)
);

CREATE OR REPLACE TRIGGER check_a
BEFORE INSERT ON attendance
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE 
	tot_class INT;
	present_no NUMBER;
	warn_count INT;
	percent NUMBER;
BEGIN
	SELECT NVL(COUNT(*),0) INTO tot_class FROM attendance WHERE roll = :n.roll AND courseno = :n.courseno;
	SELECT CAST(NVL(COUNT(*),0) AS NUMBER) INTO present_no FROM attendance WHERE roll = :n.roll AND courseno = :n.courseno AND status = 'P';
	IF (MOD(tot_class+1,5) = 0) THEN
		IF (:n.status = 'P') THEN
			present_no := present_no + 1;
		END IF;
		percent := present_no/tot_class;
		IF (percent < 0.8) THEN
			INSERT INTO attendance_warning VALUES (:n.roll, :n.adate, :n.courseno);
			SELECT COUNT(*) INTO warn_count FROM attendance_warning WHERE roll = :n.roll AND courseno = :n.courseno;
			IF (warn_count = 2) THEN
				INSERT INTO deregistration VALUES (:n.roll, :n.adate, :n.courseno);
				DELETE FROM registration WHERE roll = :n.roll AND courseno = :n.courseno;
			END IF;
		END IF;
	END IF;
END;
/

-- inserting tuples to test above trigger
INSERT INTO attendance VALUES (8, 14, date '2020-01-01', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-02', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-03', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-04', 'A'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-05', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-06', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-07', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-08', 'A'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-09', 'A'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-10', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-11', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-12', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-13', 'A'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-14', 'P'); 
INSERT INTO attendance VALUES (8, 14, date '2020-01-15', 'P'); 

SELECT * FROM attendance_warning;
SELECT * FROM deregistration;

-- iii: method 1/2
CREATE OR REPLACE FUNCTION getGrade(roll_no IN INT, sem_no IN INT)
RETURN NUMBER
IS
	weighted_sum NUMBER;
	credit_total NUMBER;
BEGIN
	-- getting the required values
	SELECT CAST(SUM(credit*igrade) AS NUMBER), CAST(SUM(credit) AS NUMBER) INTO weighted_sum, credit_total FROM 
	(((SELECT courseno FROM registration WHERE roll = roll_no AND sem_no = semester_no) NATURAL JOIN 
	(SELECT courseno, igrade FROM grade WHERE roll = roll_no)) NATURAL JOIN (SELECT courseno, credit FROM course));
	-- returning the value
	RETURN weighted_sum/credit_total;
END;
/

DECLARE 
	roll_no INT;
	sem_no INT;
BEGIN 
	roll_no := &roll_no;
	sem_no := &sem_no;
	dbms_output.put_line('The grade for the requested student in the specific semester is '||ROUND(getGrade(roll_no,sem_no),2));
END;
/

-- iv
CREATE OR REPLACE VIEW sgpa_list AS
SELECT * FROM ((SELECT roll, semester_no, ROUND(CAST(SUM(igrade*credit) AS NUMBER)/CAST(SUM(credit) AS NUMBER),2) AS sgpa FROM 
((SELECT * FROM grade) NATURAL JOIN (SELECT courseno, credit, semester_no FROM course))
GROUP BY roll, semester_no) NATURAL JOIN (SELECT roll, batch FROM student));

SELECT * FROM ((SELECT roll, name FROM student) NATURAL JOIN (SELECT * FROM sgpa_list a WHERE (a.semester_no, a.batch, a.sgpa) 
IN (SELECT semester_no, batch, MAX(sgpa) AS sgpa FROM sgpa_list b GROUP BY b.batch, b.semester_no)));

-- iii: method 2/2
DECLARE 
	roll_no INT;
	sem_no INT;
	grade NUMBER;
BEGIN 
	roll_no := &roll_no;
	sem_no := &sem_no;
	SELECT sgpa INTO grade FROM sgpa_list WHERE roll = roll_no AND semester_no = sem_no;
	dbms_output.put_line('The grade for the requested student in the specific semester is '||grade);
END;
/

-- v
CREATE OR REPLACE VIEW cgpa_list AS (SELECT * FROM ((SELECT name, roll FROM student) NATURAL JOIN
(SELECT roll, ROUND(CAST(SUM(sgpa) AS NUMBER)/COUNT(*),2) AS cgpa, batch FROM sgpa_list GROUP BY roll, batch)));

SELECT * FROM cgpa_list;

-- vi
SELECT * FROM cgpa_list a WHERE (a.batch, a.cgpa) IN (SELECT batch, MAX(cgpa) FROM cgpa_list b GROUP BY batch);

-- vii
SELECT dept, COUNT(*) AS strength, batch FROM ((SELECT roll, dept FROM student) NATURAL JOIN 
(SELECT roll, batch FROM cgpa_list WHERE cgpa > 9)) GROUP BY dept, batch;

SET SERVEROUTPUT OFF;
