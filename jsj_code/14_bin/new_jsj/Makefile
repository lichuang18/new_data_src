all:
	gcc src/recv.c -o /root/bin/recv
	gcc src/offload.c -o /root/bin/offload
	gcc src/leapDB/leapDB.c -o /root/bin/leapDB
	gcc src/leapDB/insert_n.c -o /root/bin/leapDB_bin/leapDB_insert_N -lhiredis
	gcc src/leapDB/show.c -o /root/bin/leapDB_bin/leapDB_show -lhiredis
	gcc src/leapDB/dbsize_count.c -o /root/bin/leapDB_bin/leapDB_dbsize -lhiredis
	gcc src/leapDB/clear_db.c -o /root/bin/leapDB_bin/clearDB -lhiredis
	gcc src/leapDB/count_insert.c -o /root/bin/leapDB_bin/count_insert -lhiredis
	gcc src/leapDB/select.c -o /root/bin/leapDB_bin/leapDB_select -lhiredis


clean:
	rm -f /root/bin/recv /root/bin/offload /root/bin/leapDB /root/bin/leapDB_bin/clearDB 
	rm -f /root/bin/leapDB_bin/leapDB_dbsize /root/bin/leapDB_bin/leapDB_show /root/bin/leapDB_bin/leapDB_insert_N
	rm -f /root/bin/leapDB_bin/count_insert
