-- creating all tables

CREATE TABLE student (
	name VARCHAR(30) NOT NULL,
	student_number INT PRIMARY KEY,
	class INT NOT NULL,
	major VARCHAR(4) NOT NULL
);

CREATE TABLE course (
	course_name VARCHAR(50) NOT NULL,
	course_number VARCHAR(8) PRIMARY KEY,
	credit_hours INT NOT NULL,
	department VARCHAR(4) NOT NULL
);

CREATE TABLE section (
	section_identifier INT PRIMARY KEY,
	course_number VARCHAR(8) REFERENCES course(course_number) ON DELETE CASCADE NOT NULL,
	semester VARCHAR(6) NOT NULL,
	year INT NOT NULL,
	instructor VARCHAR(30) NOT NULL 
);

CREATE TABLE grade_report (
	student_number INT REFERENCES student(student_number) NOT NULL,
	section_identifier INT NOT NULL,
	grade VARCHAR(2) NOT NULL,
	CONSTRAINT FK_grade_report FOREIGN KEY (section_identifier) REFERENCES section(section_identifier) ON DELETE CASCADE
);

CREATE TABLE prerequisite (
	course_number VARCHAR(8) NOT NULL,
	prerequisite_number VARCHAR(8) NOT NULL,
	CONSTRAINT FK1_prerequisite FOREIGN KEY (course_number) REFERENCES course(course_number) ON DELETE CASCADE,
	CONSTRAINT FK2_prerequisite FOREIGN KEY (prerequisite_number) REFERENCES course(course_number) ON DELETE CASCADE
);


-- inserting the entities into tables

INSERT ALL
INTO student (name, student_number, class, major) VALUES ('Smith', 17, 1, 'CS')
INTO student (name, student_number, class, major) VALUES ('Brown', 8, 2, 'CS')
INTO student (name, student_number, class, major) VALUES ('Pavlo', 4, 3, 'CS')
INTO student (name, student_number, class, major) VALUES ('Bieber', 2, 4, 'CS')
SELECT * FROM dual;

INSERT ALL
INTO course(course_name, course_number, credit_hours, department) VALUES ('Intro to Computer Science', 'CS1310', 4, 'CS')
INTO course(course_name, course_number, credit_hours, department) VALUES ('Data Structures', 'CS3320', 4, 'CS')
INTO course(course_name, course_number, credit_hours, department) VALUES ('Discrete Mathematics', 'MATH2410', 3, 'MATH')
INTO course(course_name, course_number, credit_hours, department) VALUES ('Database', 'CS3380', 4, 'CS')
SELECT * FROM dual;

INSERT ALL 
INTO section(section_identifier, course_number, semester, year, instructor) VALUES (85, 'MATH2410', 'Fall', 7, 'King')
INTO section(section_identifier, course_number, semester, year, instructor) VALUES (92, 'CS1310', 'Fall', 7, 'Anderson')
INTO section(section_identifier, course_number, semester, year, instructor) VALUES (102, 'CS3320', 'Spring', 8, 'Knuth')
INTO section(section_identifier, course_number, semester, year, instructor) VALUES (112, 'MATH2410', 'Fall', 8, 'Chang')
INTO section(section_identifier, course_number, semester, year, instructor) VALUES (119, 'CS1310', 'Fall', 8, 'Anderson')
INTO section(section_identifier, course_number, semester, year, instructor) VALUES (135, 'CS3380', 'Fall', 8, 'Stone')
SELECT * FROM dual;

INSERT ALL
INTO grade_report(student_number, section_identifier, grade) VALUES (17, 112, 'B')
INTO grade_report(student_number, section_identifier, grade) VALUES (17, 119, 'C')
INTO grade_report(student_number, section_identifier, grade) VALUES (8, 85, 'A')
INTO grade_report(student_number, section_identifier, grade) VALUES (8, 92, 'A')
INTO grade_report(student_number, section_identifier, grade) VALUES (8, 102, 'B')
INTO grade_report(student_number, section_identifier, grade) VALUES (8, 135, 'A')
INTO grade_report(student_number, section_identifier, grade) VALUES (4, 85, 'F')
INTO grade_report(student_number, section_identifier, grade) VALUES (4, 92, 'F')
INTO grade_report(student_number, section_identifier, grade) VALUES (4, 102, 'F')
INTO grade_report(student_number, section_identifier, grade) VALUES (2, 85, 'A')
INTO grade_report(student_number, section_identifier, grade) VALUES (2, 102, 'A')
SELECT * FROM dual;

INSERT ALL
INTO prerequisite(course_number, prerequisite_number) VALUES ('CS3380', 'CS3320')
INTO prerequisite(course_number, prerequisite_number) VALUES ('CS3380', 'MATH2410')
INTO prerequisite(course_number, prerequisite_number) VALUES ('CS3380', 'CS1310')
SELECT * FROM dual;
