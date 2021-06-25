-- Creating the tables

CREATE TABLE student (
    sroll INT PRIMARY KEY,
    sname VARCHAR(30) NOT NULL,
    gender CHAR(1) NOT NULL,
    branch CHAR(1) NOT NULL,
    programme CHAR(1) NOT NULL,
    cgpa DECIMAL(3,2)
);

CREATE TABLE company (
    company_id INT PRIMARY KEY,
    name VARCHAR(30) NOT NULL,
    specification VARCHAR(100)
);

CREATE TABLE interview (
    sroll INT REFERENCES student(sroll),
    company_id INT REFERENCES company(company_id),
    idate DATE NOT NULL
);

CREATE TABLE offer (
    sroll INT REFERENCES student(sroll),
    company_id INT REFERENCES company(company_id),
    osalary INT NOT NULL
);


/** 

    gender: 'M' - male, 'F' - female
    branch: 'A' - CSE, 'B' - CE, 'C' - ECE, 'D' - EEE, 'E' - ME, 'F' - MME
    programme: 'A' - B.Tech, 'B' - M.Tech, 'C' - M.Sc, 'D' - Ph.D
    
**/
    
    
-- Inserting values into students table 

INSERT ALL 
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (1, 'Yashwin', 'M', 'B', 'A', 9.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (2, 'Aditya', 'M', 'F', 'C', 8.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (3, 'Nitish', 'M', 'B', 'B', 9.83)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (4, 'Ramya', 'F', 'C', 'D', 6.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (5, 'Sripad', 'M', 'B', 'C', 9.99)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (6, 'Kiran', 'M', 'A', 'A', 7.82)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (7, 'Sanath', 'M', 'A', 'A', 9.43)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (8, 'Saketh', 'M', 'D', 'C', 7.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (9, 'Vyshnavi', 'F', 'F', 'A', 6.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (10, 'Sureka', 'F', 'A', 'D', 9.13)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (11, 'Ariyana', 'F', 'B', 'C', 8.26)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (12, 'Bindu', 'F', 'B', 'A', 7.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (13, 'Shreya Ghoshal', 'F', 'A', 'A', 8.16)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (14, 'Deepika Padukone', 'F', 'A', 'C', 7.89)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (15, 'Madhuri Dixit', 'F', 'C', 'A', 8.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (16, 'Shahid Kapoor', 'M', 'B', 'C', 9.65)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (17, 'Sudheer', 'M', 'D', 'C', 9.83)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (18, 'Lasya', 'F', 'D', 'A', 8.67)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (19, 'Pranavi', 'F', 'D', 'A', 8.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (20, 'Praveen', 'M', 'E', 'B', 6.23)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (21, 'Swetha', 'F', 'E', 'B', 6.13)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (22, 'Manish', 'M', 'E', 'B', 9.67)
INTO student (sroll, sname, gender, branch, programme, cgpa) VALUES (23, 'Nethra', 'F', 'A', 'A', 8.93)
SELECT * FROM dual;


/** Inserting values into company table **/

INSERT ALL
INTO company (company_id, name, specification) VALUES (1, 'Google', 'The top tech giant.')
INTO company (company_id, name, specification) VALUES (2, 'Amazon', 'The top e-commerce enterprise.')
INTO company (company_id, name, specification) VALUES (3, 'Facebook', 'The top social-networking platform.')
INTO company (company_id, name, specification) VALUES (4, 'Netflix', 'The top OTT platform')
INTO company (company_id, name, specification) VALUES (5, 'Apple', 'The top device manufacturer.')
SELECT * FROM dual;


/** Inserting values into interview table **/

INSERT ALL
INTO interview (sroll, company_id, idate) VALUES (1, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (2, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (3, 3, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (4, 4, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (5, 5, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (6, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (7, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (8, 3, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (9, 4, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (10, 5, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (11, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (12, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (13, 3, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (14, 4, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (15, 5, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (16, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (17, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (18, 3, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (19, 4, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (20, 5, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (1, 4, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (2, 5, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (3, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (4, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (5, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (6, 3, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (7, 4, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (8, 5, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (9, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (10, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (11, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (12, 2, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
INTO interview (sroll, company_id, idate) VALUES (13, 1, TO_DATE('12/01/2016', 'DD/MM/YYYY'))
SELECT * FROM dual;


/** Inserting values into offer table **/

INSERT ALL
INTO offer (sroll, company_id, osalary) VALUES (1, 1, 20000)
INTO offer (sroll, company_id, osalary) VALUES (2, 2, 200000)
INTO offer (sroll, company_id, osalary) VALUES (3, 3, 50000)
INTO offer (sroll, company_id, osalary) VALUES (4, 4, 600000)
INTO offer (sroll, company_id, osalary) VALUES (5, 5, 400000)
INTO offer (sroll, company_id, osalary) VALUES (1, 4, 20000)
SELECT * FROM dual;
