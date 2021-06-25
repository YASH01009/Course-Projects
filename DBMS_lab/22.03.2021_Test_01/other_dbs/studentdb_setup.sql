CREATE TABLE student (
    roll INT,
    name VARCHAR(32),
    dept CHAR(2),
    batch INT,
    email VARCHAR(8),
    PRIMARY KEY (roll)
);

CREATE TABLE course (
    courseno INT,
    cname VARCHAR(32),
    offering_dept CHAR(2),
    semester_no INT,
    course_type CHAR(1),
    credit INT,
    PRIMARY KEY (courseno)
);

CREATE TABLE prerequisite (
    courseno INT,
    prcourseno INT,
    PRIMARY KEY (courseno, prcourseno),
    FOREIGN KEY (courseno) REFERENCES course(courseno),
    FOREIGN KEY (prcourseno) REFERENCES course(courseno)
);

CREATE TABLE registration (
    roll INT,
    courseno INT,
    semester_no INT,
    PRIMARY KEY (roll, courseno),
    FOREIGN KEY (roll) REFERENCES student (roll),
    FOREIGN KEY (courseno) REFERENCES course (courseno)
);

CREATE TABLE grade (
    roll INT,
    courseno INT,
    igrade INT,
    PRIMARY KEY(roll, courseno),
    FOREIGN KEY(roll, courseno) REFERENCES registration(roll, courseno)
);

CREATE TABLE attendance (
    roll INT,
    courseno INT,
    adate DATE,
    status CHAR,
    PRIMARY KEY (roll, courseno, adate)
);

-- inserting tuples
INSERT ALL
INTO student (roll, name, dept, batch, email) VALUES (1, 'Andrew Pavlo', 'CS', 2020, 'ap11')
INTO student (roll, name, dept, batch, email) VALUES (2, 'James Musk', 'CS', 2020, 'jm23')
INTO student (roll, name, dept, batch, email) VALUES (3, 'Richard Jackson', 'CS', 2020, 'rj32')
INTO student (roll, name, dept, batch, email) VALUES (4, 'Jack Lite', 'ME', 2020, 'jl12')
INTO student (roll, name, dept, batch, email) VALUES (5, 'Mike Tyson', 'ME', 2020, 'mk34')
INTO student (roll, name, dept, batch, email) VALUES (6, 'Donald Trump', 'ME', 2021, 'dt34')
INTO student (roll, name, dept, batch, email) VALUES (7, 'Joe Biden', 'ME', 2021, 'jb28')
SELECT * FROM dual;

-- T: Theory, E: Elective, L: Laboratory
INSERT ALL
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (1, 'Data Structures', 'CS', 1, 'T', 3)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (2, 'Design of Algorithms', 'CS', 1, 'E', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (3, 'Mechanics of Solids', 'ME', 1, 'T',4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (4, 'Kinematics - I', 'ME', 1, 'E',3)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (5, 'Computer Networks', 'CS', 2, 'T', 3)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (6, 'Database Systems Lab', 'CS', 2, 'L', 2)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (7, 'Operating Systems', 'CS', 2, 'E', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (8, 'Traffic Enginnering', 'ME', 2, 'T', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (9, 'Kinematics - II', 'ME', 2, 'E', 4)
INTO course (courseno, cname, offering_dept, semester_no, course_type, credit) VALUES (10, 'Heat Transfer Workshop', 'ME', 2, 'L', 2)
SELECT * FROM dual;

INSERT ALL
INTO registration (roll, courseno, semester_no) VALUES (1, 1, 1)
INTO registration (roll, courseno, semester_no) VALUES (1, 2, 1)
INTO registration (roll, courseno, semester_no) VALUES (1, 5, 2)
INTO registration (roll, courseno, semester_no) VALUES (2, 1, 1)
INTO registration (roll, courseno, semester_no) VALUES (2, 5, 2)
INTO registration (roll, courseno, semester_no) VALUES (2, 6, 2)
INTO registration (roll, courseno, semester_no) VALUES (3, 2, 1)
INTO registration (roll, courseno, semester_no) VALUES (3, 5, 2)
INTO registration (roll, courseno, semester_no) VALUES (3, 7, 2)
INTO registration (roll, courseno, semester_no) VALUES (4, 3, 1)
INTO registration (roll, courseno, semester_no) VALUES (4, 4, 1)
INTO registration (roll, courseno, semester_no) VALUES (4, 8, 2)
INTO registration (roll, courseno, semester_no) VALUES (5, 4, 1)
INTO registration (roll, courseno, semester_no) VALUES (5, 9, 2)
INTO registration (roll, courseno, semester_no) VALUES (5, 10, 2)
INTO registration (roll, courseno, semester_no) VALUES (6, 3, 1)
INTO registration (roll, courseno, semester_no) VALUES (7, 3, 1)
SELECT * FROM dual;

INSERT ALL
INTO attendance (roll, courseno, adate, status) VALUES (1, 1, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (1, 1, date '2020-01-02', 'A')
INTO attendance (roll, courseno, adate, status) VALUES (2, 1, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (2, 1, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (1, 2, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (1, 2, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (3, 2, date '2020-01-01', 'A')
INTO attendance (roll, courseno, adate, status) VALUES (3, 2, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (4, 3, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (4, 3, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (4, 4, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (4, 4, date '2020-01-02', 'A')
INTO attendance (roll, courseno, adate, status) VALUES (5, 4, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (5, 4, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (1, 5, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (1, 5, date '2020-01-02', 'A')
INTO attendance (roll, courseno, adate, status) VALUES (2, 5, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (2, 5, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (3, 5, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (3, 5, date '2020-01-02', 'A')
INTO attendance (roll, courseno, adate, status) VALUES (2, 6, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (2, 6, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (3, 7, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (3, 7, date '2020-01-02', 'A')
INTO attendance (roll, courseno, adate, status) VALUES (4, 8, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (4, 8, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (5, 9, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (5, 9, date '2020-01-02', 'A')
INTO attendance (roll, courseno, adate, status) VALUES (5, 10, date '2020-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (5, 10, date '2020-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (6, 3, date '2021-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (6, 3, date '2021-01-02', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (7, 3, date '2021-01-01', 'P')
INTO attendance (roll, courseno, adate, status) VALUES (7, 3, date '2021-01-02', 'P')
SELECT * FROM dual;

INSERT ALL
INTO grade (roll, courseno, igrade) VALUES (1, 1, 9)
INTO grade (roll, courseno, igrade) VALUES (1, 2, 8)
INTO grade (roll, courseno, igrade) VALUES (1, 5, 7)
INTO grade (roll, courseno, igrade) VALUES (2, 1, 6)
INTO grade (roll, courseno, igrade) VALUES (2, 5, 7)
INTO grade (roll, courseno, igrade) VALUES (2, 6, 7)
INTO grade (roll, courseno, igrade) VALUES (3, 2, 8)
INTO grade (roll, courseno, igrade) VALUES (3, 5, 9)
INTO grade (roll, courseno, igrade) VALUES (3, 7, 6)
INTO grade (roll, courseno, igrade) VALUES (4, 3, 7)
INTO grade (roll, courseno, igrade) VALUES (4, 4, 7)
INTO grade (roll, courseno, igrade) VALUES (4, 8, 8)
INTO grade (roll, courseno, igrade) VALUES (5, 4, 9)
INTO grade (roll, courseno, igrade) VALUES (5, 9, 10)
INTO grade (roll, courseno, igrade) VALUES (5, 10, 9)
INTO grade (roll, courseno, igrade) VALUES (6, 3, 9)
INTO grade (roll, courseno, igrade) VALUES (7, 3, 8)
SELECT * FROM dual;

INSERT ALL
INTO prerequisite (courseno, prcourseno) VALUES(5, 1)
INTO prerequisite (courseno, prcourseno) VALUES(8, 3)
SELECT * FROM dual;
