repair table mysql.spider_tables;
set @@session.spider_delete_all_rows_type = 0;

echo #######################################################################################
echo $test_des;
echo spider_casual_read_value=$spider_casual_read_value;
echo spider_bgs_mode_value=$spider_bgs_mode_value;

--disable_warnings
repair table mysql.spider_tables;
eval drop table if exists $test_table;
--enable_warnings


--disable_query_log
eval set spider_bgs_first_read=9223372036854775807;
eval set spider_bgs_second_read=9223372036854775807;
--enable_query_log

#
# 建表
#
--disable_query_log
eval $create_table_sql;
--enable_query_log
#
# clear
#
eval truncate table $test_table;


#
# 获取空结果集
# bug_dm支持空结果集
#
--sorted_result
eval Select * from $test_table;

#
# 插入测试
# 
#

eval insert into $test_table values(1, 1.1, 'aaaa1', 'bbbbbb1', null);
eval insert into $test_table values(21, 2, 'aaaa2', 'bbbbbb2', '2011-1-1');
eval insert into $test_table(a) values(300);
eval insert into $test_table values(230, 0.0001, 'aaaa3', 'bbbbbb3', '2012-2-2');

--sorted_result
eval Select * from $test_table;

#
# order by 
#
eval Select A, B, C, D, E from $test_table order by A;
eval Select A, B, C, D, E from $test_table order by B,A;
eval Select A, B, C, D, E from $test_table order by C,A;
eval Select A, B, C, D, E from $test_table order by D,A;
eval Select A, B, C, D, E from $test_table order by E,A;


#
# statics group having limit
#
eval Select count(A),avg(A),sum(A),min(A),max(A) from $test_table;
eval Select C, count(A),avg(A),sum(A),min(A),max(A) from $test_table group by C;
eval Select C, count(A),avg(A),sum(A),min(A),max(A) from $test_table group by C having count(A) =2;

--sorted_result
eval Select * from $test_table order by A limit 1,2;

#
# 单表测试，带入各行的条件
#
#
#ora 
#1
--sorted_result
eval Select A, B, C, D, E from $test_table where A=1;
#Select A, B, C, D, E from $test_table where B=1.1;
--sorted_result
eval Select A, B, C, D, E from $test_table where C='aaaa1';
--sorted_result
eval Select A, B, C, D, E from $test_table where D='bbbbbb1';
--sorted_result
eval Select A, B, C, D, E from $test_table where E is null;
#2
--sorted_result
eval Select A, B, C, D, E from $test_table where A=21;
#Select A, B, C, D, E from $test_table where B=1.1;
--sorted_result
eval Select A, B, C, D, E from $test_table where C='aaaa2';
--sorted_result
eval Select A, B, C, D, E from $test_table where D='bbbbbb2';
--sorted_result
eval Select A, B, C, D, E from $test_table where E='2011-1-1';
#3
--sorted_result
eval Select A, B, C, D, E from $test_table where B is null and c is null and d is null and e is null;



#
# 更新测试
# 
#
--disable_warnings
eval update $test_table  set A=A+1, C='jack1', D='english1', E= '2011-1-1' where A=1;
--enable_warnings

--sorted_result
eval Select * from $test_table;


#
# delete test
# 
#
eval delete from $test_table  where A=2;

--sorted_result
eval Select * from $test_table;
