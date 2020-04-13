create table pusers(id number(3) primary key ,userid varchar2(12),password varcharr2(16);

create table messages (sender number(3), reciever number(3), message varchar2(100), timestamp date );

alter session set nls_date_format = 'hh24:mi:ss';

alter table messages ADD CONSTRAINT fk1  FOREIGN KEY (sender)  REFERENCES pusers (id);
alter table messages ADD CONSTRAINT fk1  FOREIGN KEY (reciever)  REFERENCES pusers (id);



insert into pusers values(101,'user1','password1');
insert into pusers values(102,'user2','password2');
insert into pusers values(103,'user3','password3');
insert into pusers values(104,'user4','password4');
insert into pusers values(105,'user5','password5');


commit;


insert into messages values(101,102,'message212',sysdate);
insert into messages values(101,103,'message2123',sysdate);
insert into messages values(101,104,'message565',sysdate);
insert into messages values(101,105,'message565',sysdate);
insert into messages values(102,101,'messag65e',sysdate);
insert into messages values(102,103,'message2323',sysdate);
insert into messages values(102,104,'message23',sysdate);
insert into messages values(102,105,'message434',sysdate);
insert into messages values(103,101,'message434',sysdate);
insert into messages values(103,102,'message32',sysdate);
insert into messages values(103,104,'message121',sysdate);
insert into messages values(103,105,'message1001',sysdate);
insert into messages values(104,101,'message100',sysdate);
insert into messages values(104,102,'message9',sysdate);
insert into messages values(104,103,'message8',sysdate);
insert into messages values(104,105,'message7',sysdate);
insert into messages values(105,101,'message6',sysdate);
insert into messages values(105,102,'message5',sysdate);
insert into messages values(105,103,'message4',sysdate);
insert into messages values(105,104,'message3',sysdate);


commit;





