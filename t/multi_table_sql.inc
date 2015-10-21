repair table mysql.spider_tables;

echo #######################################################################################
echo $test_des;
echo $spider_casual_read_value;
echo $spider_bgs_mode_value;

eval set spider_bgs_mode=$spider_bgs_mode_value;
eval set spider_casual_read=$spider_casual_read_value;

--disable_query_log
eval set spider_bgs_first_read=9223372036854775807;
eval set spider_bgs_second_read=9223372036854775807;
--enable_query_log


--disable_warnings
eval drop table if exists $test_table_s,$test_table_c,$test_table_sc;
--enable_warnings

--disable_query_log
eval $create_s_sql;
eval $create_c_sql;
eval $create_sc_sql;
--enable_query_log
#
# clear
#
eval truncate table $test_table_s; 
eval truncate table $test_table_c;
eval truncate table $test_table_sc;

#
# 获取空结果集
#
--sorted_result
eval Select * from $test_table_s; 
--sorted_result
eval Select * from $test_table_c;
--sorted_result
eval Select * from $test_table_sc;

#
# 插入测试
# 
#

eval INSERT INTO $test_table_s(sno, sname, sdept, age) VALUES(1001, 'ZhangQiang','DianZi',18);
eval INSERT INTO $test_table_s(sno, sname, sdept, age) VALUES(1002, 'WangHong','DianZi',20);
eval INSERT INTO $test_table_s(sno, sname, sdept, age) VALUES(2001, 'LiLi','TiYu',25);
eval INSERT INTO $test_table_s(sno, sname, sdept, age) VALUES(2002, 'ZhangJian','TiYu',26);

eval INSERT INTO $test_table_c(cno, cname, pcno) VALUES(11, 'Math',NULL);
eval INSERT INTO $test_table_c(cno, cname, pcno) VALUES(12, 'Computer',11);
eval INSERT INTO $test_table_c(cno, cname, pcno) VALUES(61, 'Eng',NULL);
eval INSERT INTO $test_table_c(cno, cname, pcno) VALUES(62, 'Athe',61);

eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(1001, 11, 90);
eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(1001, 12, 85);
eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(1002, 61, 80);
eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(1002, 11, 79);
eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(2001, 12, 68);
eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(2001, 62, 90);
eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(2002, 62, 88);
eval INSERT INTO $test_table_sc(SNO, CNO, GRADE) VALUES(2002, 61, 72);

--sorted_result
eval Select * from $test_table_s; 
--sorted_result
eval Select * from $test_table_c;
--sorted_result
eval Select * from $test_table_sc;


#
# multiple table query
# 
#



--sorted_result
eval select $test_table_s.sno, $test_table_sc.cno, sname, grade from $test_table_s, $test_table_sc where $test_table_s.sno = $test_table_sc.sno;
--sorted_result
eval select $test_table_c.cno, $test_table_c.cname, $test_table_sc.sno, $test_table_sc.grade from $test_table_c, $test_table_sc where $test_table_c.cno = $test_table_sc.cno;
--sorted_result
eval select $test_table_s.sno, $test_table_s.sname, $test_table_c.cno, $test_table_c.cname, grade from $test_table_s, $test_table_sc, $test_table_c where $test_table_s.sno = $test_table_sc.sno and $test_table_c.cno = $test_table_sc.cno;

--sorted_result
eval select * from $test_table_s where sno in (select sno from $test_table_sc where cno in (select cno from $test_table_c where cname='Math'));
--sorted_result
eval select * from $test_table_sc x where grade >= (SELECT avg(grade) from $test_table_sc y where y.sno = x.sno);
--sorted_result
eval SELECT * from $test_table_s where EXISTS(select * from $test_table_sc where $test_table_sc.cno =11 and $test_table_s.sno = $test_table_sc.sno);
