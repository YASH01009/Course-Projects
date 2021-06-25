-- i
SELECT DISTINCT name, skill, ph_no FROM migrantw WHERE skill = 'Electrician' OR skill = 'TV Mechanic';

-- ii
SELECT DISTINCT name FROM migrantw WHERE pjobloc = 'Bangalore' AND skill = 'Security Gaurd';

-- iii
CREATE OR REPLACE VIEW pjobloc_view AS SELECT name, skill, pjobloc FROM migrantw WHERE pjobloc = 'Noida' OR pjobloc = 'Delhi' ORDER BY skill;
SELECT * FROM pjobloc_view;

-- v
SELECT address, COUNT(DISTINCT enroll_id) AS strength FROM migrantw GROUP BY address;

-- vi
CREATE TABLE company_req (
	cname VARCHAR(32),
	jobtype VARCHAR(32),
	jobloc VARCHAR(32),
	vacancy NUMBER NOT NULL,
	skill_req VARCHAR(32) NOT NULL,
	CONSTRAINT comp_req_pk PRIMARY KEY (cname, jobtype, jobloc)
);

CREATE TABLE place_migrant (
	cname VARCHAR(32) NOT NULL,
	enroll_id NUMBER NOT NULL,
	jobtype VARCHAR(32) NOT NULL,
	osalary NUMBER NOT NULL,
	jobloc VARCHAR(32) NOT NULL,
	place_date DATE NOT NULL
);

CREATE OR REPLACE TRIGGER place_workers
BEFORE INSERT ON company_req
REFERENCING NEW AS n OLD AS o 
FOR EACH ROW
DECLARE
	tenroll_id migrantw.enroll_id%TYPE;
	tpjobloc migrantw.pjobloc%TYPE;
	tskill migrantw.skill%TYPE;
	CURSOR c IS SELECT enroll_id, pjobloc, skill FROM migrantw WHERE status = 'U' ORDER BY enroll_date;
BEGIN
	OPEN c;
	LOOP 
		FETCH c INTO tenroll_id, tpjobloc, tskill;
		EXIT WHEN c%NOTFOUND;
		IF tpjobloc = :n.jobloc AND tskill = :n.skill_req AND :n.vacancy > 0 THEN
			-- insert the details into place_migrant relation
			INSERT INTO place_migrant VALUES (:n.cname, tenroll_id, :n.jobtype, 500, :n.jobloc, SYSDATE);
			-- mark the worker placed in migrantw
			UPDATE migrantw SET status = 'P' WHERE enroll_id = tenroll_id;
			:n.vacancy := :n.vacancy - 1;
		END IF;
	END LOOP;
	CLOSE c;
END;
/

INSERT INTO company_req VALUES ('Bajaj', 'Full time', 'Mumbai', 3, 'Electrician');

SELECT * FROM company_req;
SELECT * FROM place_migrant;
SELECT DISTINCT enroll_id, status FROM migrantw;

-- vii
CREATE OR REPLACE VIEW placed_migrants_view AS SELECT * FROM 
((SELECT DISTINCT name, ph_no, enroll_id FROM migrantw) NATURAL JOIN 
(SELECT cname, osalary, jobloc, place_date, enroll_id FROM place_migrant));
SELECT * FROM placed_migrants_view;

-- viii
SELECT cname, jobtype, jobloc, vacancy FROM company_req WHERE vacancy > 0; 
