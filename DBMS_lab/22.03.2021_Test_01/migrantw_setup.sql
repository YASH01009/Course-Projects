CREATE TABLE migrantw (	
	enroll_id NUMBER, 
	name VARCHAR(64) NOT NULL, 
	skill VARCHAR2(64), 
	address VARCHAR(32) NOT NULL, 
	pjobloc VARCHAR2(64), 
	ph_no CHAR(10) NOT NULL, 
	enroll_date DATE NOT NULL,
	status CHAR(1) NOT NULL,
	PRIMARY KEY (enroll_id, skill, pjobloc)
);

INSERT INTO migrantw VALUES (1, 'Ruchit Jain', 'TV Mechanic', 'Karnataka', 'Mumbai', '9876543210', DATE '2020-03-13', 'U');
INSERT INTO migrantw VALUES (1, 'Ruchit Jain', 'TV Mechanic', 'Karnataka', 'Bangalore', '9876543210', DATE '2020-03-13', 'U');
INSERT INTO migrantw VALUES (1, 'Ruchit Jain', 'TV Mechanic', 'Karnataka', 'Delhi', '9876543210', DATE '2020-03-13', 'U');
INSERT INTO migrantw VALUES (1, 'Ruchit Jain', 'Electrician', 'Karnataka', 'Mumbai', '9876543210', DATE '2020-03-13', 'U');
INSERT INTO migrantw VALUES (1, 'Ruchit Jain', 'Electrician', 'Karnataka', 'Bangalore', '9876543210', DATE '2020-03-13', 'U');
INSERT INTO migrantw VALUES (1, 'Ruchit Jain', 'Electrician', 'Karnataka', 'Delhi', '9876543210', DATE '2020-03-13', 'U');
INSERT INTO migrantw VALUES (2, 'Pranab Jaiswal', 'Electrician', 'Maharastra', 'Mumbai', '9898656532', DATE '2020-04-12', 'U');
INSERT INTO migrantw VALUES (3, 'Ankit Singh', 'TV Mechanic', 'Tamil Nadu', 'Mumbai', '9960754382', DATE '2020-03-23', 'U');
INSERT INTO migrantw VALUES (4, 'Nalin Gowd', 'Security Gaurd', 'Maharastra', 'Mumbai', '9473205723', DATE '2020-01-30', 'U');
INSERT INTO migrantw VALUES (4, 'Nalin Gowd', 'Security Gaurd', 'Maharastra', 'Bangalore', '9473205723', DATE '2020-01-30', 'U');
INSERT INTO migrantw VALUES (5, 'Prakash Janwar', 'Cab Driver', 'Uttar Pradesh', 'Mumbai', '8196385035', DATE '2020-05-17', 'U');
INSERT INTO migrantw VALUES (5, 'Prakash Janwar', 'Security Gaurd', 'Uttar Pradesh', 'Mumbai', '8196385035', DATE '2020-05-17', 'U');
INSERT INTO migrantw VALUES (6, 'Pramod Lolak', 'Carpenter', 'Karnataka', 'Mumbai', '9539667282', DATE '2020-06-21', 'U');
