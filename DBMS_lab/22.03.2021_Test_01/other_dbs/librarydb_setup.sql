CREATE TABLE bookstock(
    bookid INT,
    title VARCHAR(32),
    noofcopies INT,
    PRIMARY KEY (bookid)
);

CREATE TABLE bookissue(
    trid INT,
    cardno INT,
    choldername VARCHAR(32),
    bookid INT,
    issuedate DATE,
    DUEDATE DATE,
    PRIMARY KEY (trid),
    CONSTRAINT fk_bid
    FOREIGN KEY (bookid) REFERENCES bookstock(bookid) ON DELETE CASCADE
);

CREATE TABLE bookreturn(
    trid INT,
    cardno INT,
    choldername VARCHAR(32),
    bookid INT,
    RETURNDATE DATE,
    issuedate DATE,
    PRIMARY KEY (trid),
    CONSTRAINT fk_bid2
    FOREIGN KEY (bookid) REFERENCES bookstock(bookid) ON DELETE CASCADE
);


CREATE SEQUENCE trid_seq
START WITH 1
INCREMENT BY 1
NO CACHE
NO CYCLE;

INSERT INTO bookstock VALUES(1, 'BOOK1', 10);
INSERT INTO bookstock VALUES(2, 'BOOK2', 10);
INSERT INTO bookstock VALUES(3, 'BOOK3', 10);
INSERT INTO bookstock VALUES(4, 'BOOK4', 1);

SELECT * FROM bookstock;
SELECT * FROM bookissue;
SELECT * FROM bookreturn;


CREATE TABLE fine (
    cardno INT, 
    amt INT
);
