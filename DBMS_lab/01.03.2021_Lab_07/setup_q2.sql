-- creating tables

CREATE TABLE student (
	sroll INT,
	name VARCHAR(32)
	CONSTRAINT sname_nn NOT NULL,
	branch CHAR(2)
	CONSTRAINT branch_nn NOT NULL,
	batch INT
	CONSTRAINT batch_nn NOT NULL,
	programme CHAR(1)
	CONSTRAINT programme_nn NOT NULL
);

ALTER TABLE student ADD (
	CONSTRAINT student_pk PRIMARY KEY (sroll)
);

INSERT ALL
INTO student (sroll, name, branch, batch, programme) VALUES (21, 'Mike Tyson', 'CS', 2021, 'B')
INTO student (sroll, name, branch, batch, programme) VALUES (34, 'David Warner', 'CS', 2021, 'B')
INTO student (sroll, name, branch, batch, programme) VALUES (42, 'Yuzvendra Chahal', 'CS', 2021, 'B')
INTO student (sroll, name, branch, batch, programme) VALUES (56, 'Rohit Sharma', 'CS', 2021, 'B')
INTO student (sroll, name, branch, batch, programme) VALUES (79, 'Jasprit Bumrah', 'CS', 2021, 'B')
SELECT * FROM dual;

CREATE TABLE course (
	cid INT,
	cname VARCHAR(32)
	CONSTRAINT cname_nn NOT NULL,
	instructor VARCHAR(32)
	CONSTRAINT instructor_nn NOT NULL
);

ALTER TABLE course ADD (
	CONSTRAINT course_pk PRIMARY KEY (cid)
);

INSERT ALL
INTO course (cid, cname, instructor) VALUES (33, 'Computer Networks', 'Nagarjuna Akkineni')
INTO course (cid, cname, instructor) VALUES (55, 'Database Systems', 'Sundeep Kishan')
SELECT * FROM dual;

CREATE TABLE attendance (
	sroll INT,
	course_id INT,
	start_date DATE
	CONSTRAINT start_date_nn NOT NULL,
	end_date DATE
	CONSTRAINT end_date_nn NOT NULL,
	total_class INT
	CONSTRAINT total_class_nn NOT NULL,
	attended INT
	CONSTRAINT attended_nn NOT NULL
);

ALTER TABLE attendance ADD (
	CONSTRAINT attendance_pk PRIMARY KEY (sroll, course_id),
	CONSTRAINT atten_sroll_fk FOREIGN KEY (sroll) REFERENCES student,
	CONSTRAINT atten_cid_fk FOREIGN KEY (course_id) REFERENCES course
);

INSERT ALL
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (21, 33, DATE '2021-02-16', DATE '2021-03-16', 10, 5)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (34, 33, DATE '2021-02-16', DATE '2021-03-16', 10, 6)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (42, 33, DATE '2021-02-16', DATE '2021-03-16', 10, 7)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (56, 33, DATE '2021-02-16', DATE '2021-03-16', 10, 8)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (79, 33, DATE '2021-02-16', DATE '2021-03-16', 10, 9)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (21, 55, DATE '2021-02-16', DATE '2021-03-16', 10, 9)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (34, 55, DATE '2021-02-16', DATE '2021-03-16', 10, 8)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (42, 55, DATE '2021-02-16', DATE '2021-03-16', 10, 7)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (56, 55, DATE '2021-02-16', DATE '2021-03-16', 10, 6)
INTO attendance (sroll, course_id, start_date, end_date, total_class, attended) VALUES (79, 55, DATE '2021-02-16', DATE '2021-03-16', 10, 5)
SELECT * FROM dual;
