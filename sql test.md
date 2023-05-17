```sql
create database drj;
use drj;
create table student(sno string,sname string, age int, address string) constraint c1 primary key sno;
create table course(cno string, cname string) constraint c2 primary key cno;
create table sc(sno string, cno string, grade int) constraint c3 foreign key sno references student sno
constraint c4 foreign key cno references course cno;

insert into student values(21301032, drj, 20, beijing);
insert into student values(21301114, yxh, 200, beijing);
insert into student values(21301011, gyf, 123, nanjing);

insert into course values(1, eat);
insert into course values(2, sleep);
insert into course values(3, study);

insert into sc values(21301032, 1, 100);

delete from student where sno = 21301032;


```

