<?php
class Model {

    /* 
     * 表名称
     */
    public $_tableName = NULL;

    /*
     * 数据库连接句柄
     */
    public $_con = NULL;

    /*
     *  表主键
     */
    public $_tablePk = NULL; 

    /*
     * 记录执行的sql语句
     */
    public $arrSql;

    function __construct() {
        global $CONFIGURE;
        $this->_con = mysql_connect($CONFIGURE['db']['host'], $CONFIGURE['db']['user'], $CONFIGURE['db']['password']);
		mysql_query("SET NAMES UTF8");
        $result = mysql_select_db($CONFIGURE['db']['database'], $this->_con);
    }

    /**
     * 执行一个sql语句
     *
     * @param sql 需要执行的sql语句
     *
     */
    private function exec( $sql )
    {
        $this->arrSql[] = $sql;
        if ( $result = mysql_query($sql, $this->_con) )
        {
            return $result;
        }
        else
        {
            printErrorLog("{$sql} 执行错误:".mysql_error()); 
			return $result;
        }
    }

    /**
     * 按sql语句获取记录结果，返回数组
     *
     * @param sql 执行的sql语句
     *
     */
    private function getArray( $sql )
    {
        if ( ! $result = $this->exec($sql) ) 
            return array();
        if ( ! mysql_num_rows($result) )
            return array();
        $rows = array();
        while ( $rows[] =  mysql_fetch_array($result, MYSQL_ASSOC) ){}
        mysql_free_result($result);
        array_pop($rows);
        return $rows; 
    }

    /**
     * 格式化带limit的sql语句
     */ 
    private function setlimit( $sql, $limit )
    {
        return $sql. " LIMIT {$limit}";
    }



    /**
     * 返回影响的行数
     */
    private function affected_rows()
    {
        return mysql_affected_rows($this->_con);
    }

    /**
     * 获取数据表结构
     *
     * @param tbl_name 表名称
     */
    private function getTable( $tbl_name )
    {
        return $this->getArray("DESCRIBE {$tbl_name}");
    }
    /**
     * 过滤特殊字符
     *
     * @param value 值
     */
    private function escape($value)    
    {
        if ( is_null($value) )  
            return 'NULL';
        if ( is_bool($value) ) 
            return $value ? 1 : 0;
        if ( is_int($value) )
            return (int)$value;
        if ( is_float($value))
            return (float)$value;
        if ( @get_magic_quotes_gpc() )
            $value = stripslashes($value);
        return '\''.mysql_real_escape_string($value, $this->_con).'\'';
    }


    /**
     * 按表字段调整合适的字段
     * @param rows 输入的表字段
    private function preFormat( $rows )
    {
        $columns = $this->getTable($this->_tableName);
        $newcol = array();
        foreach ($columns as $col)
        {
            $newcol[$col['Field']] = $col['Field'];
        }
        return array_intersect_key($rows, $newcol);
    }
     */

    /**
     * 返回当前插入记录的主键的ID
     */
    private function newinsertid()
    {
        return mysql_insert_id($this->_con);
    }

    /**
     * 从数据表中查找一条记录
     *  @param conditions    查找条件，数组array("字段名"=>"查找值")或字符串，
     *  请注意在使用字符串时将需要开发者自行使用escape来对输入值进行过滤
     *  @param sort    排序，等同于“ORDER BY ”
     *  @param fields    返回的字段范围，默认为返回全部字段的值
     *                  
     **/
    public function find($conditions = NULL, $sort = NULL, $fields = NULL)
    {
        $conditions;
        if( $record = $this->findAll($conditions, $sort, $fields, 1) ){
            return array_pop($record);
        }
        else
        {
            return FALSE;
        }
    }

    /**
     *  从数据表中查找记录
     *
     * @param conditions 查找条件，数组array("字段名"=>"查找值")或字符串，
     * @param sort       排序，等同于“ORDER BY ”
     * @param fields     返回的字段范围，默认为返回全部字段的值
     * @param limit      返回的结果数量限制，等同于“LIMIT ”，如$limit = " 3, 5"，即是从第3条记录（从0开始计算）开始获取，共获取5条记录
     *
     *  如果limit值只有一个数字，则是指代从0条记录开始。
     */

    public function findAll($conditions = NULL, $sort = NULL, $fields = NULL, $limit = NULL)
    {
        $where = "";
        $fields = empty($fields) ? "*" : $fields;

        if ( is_array($conditions) )
        {
            $join = array();
            foreach ( $conditions as $key => $condition )
            {
                $condition = $this->escape($condition);
                $join[] = "{$key} = {$condition}";
            }
            $where = "WHERE ".join(" AND ",$join);
        }
        else
        {
            if( NULL != $conditions )
            {
                $where = "WHERE ".$conditions;
            }
        }

        if( NULL!= $sort )
        {
            $sort = "ORDER BY {$sort}";
        }
        else
        {
            $sort = "ORDER BY {$this->_tablePk}";
        }

        $sql = "SELECT {$fields} FROM {$this->_tableName} {$where} {$sort}";
        if( NULL != $limit )
            $sql = $this->setlimit($sql, $limit);

        return $this->getArray($sql);
    }


    /**
     * 按字段值查找一条记录
     *
     * @param field 字符串，对应数据表中的字段名
     * @param value 字符串，对应的值
     *
     */

    public function findBy($field, $value)
    {
        $tmp="field=".$field."value=".$value;;
		
        return $this->find(array($field=>$value));
    }
    /**
     * 按字段值修改一条记录
     *
     * @param conditions 数组形式，查找条件，此参数的格式用法与find/findAll的查找条件参数是相同的。
     * @param field 字符串，对应数据表中的需要修改的字段名
     * @param value 字符串，新值
     *
     */



    /**
     *  在数据表中新增一行数据
     *
     *  @param row 数组形式，数组的键是数据表中的字段名，键对应的值是需要新增的数据。
     *
     */

    public function add($row)
    {
        if(!is_array($row))return FALSE;
       // $row = $this->preFormat($row);
        if(empty($row))return FALSE;
        foreach($row as $key => $value){
            $cols[] = $key;
            $vals[] = $this->escape($value);
        }
        $col = join(',', $cols);
        $val = join(',', $vals);

        $sql = "INSERT INTO {$this->_tableName} ({$col}) VALUES ({$val})";
        
        $exeres = $this->exec($sql);  
        if( FALSE !=  $exeres/*$this->exec($sql)*/ )
        { // 获取当前新增的ID
            if( $newinserid = $this->newinsertid() )
            {
                return $newinserid;
            }
            else
            {
                return array_pop( $this->find($row, "{$this->_tablePk} DESC",$this->_tablePk) );
            }
        }
        return FALSE;
    }


    /**
     * 在数据表中新增多条记录
     *
     * @param rows 数组形式，每项均为add的$row的一个数组
     *
     */

    public function addAll($rows)
    {
        foreach($rows as $row){ 
            if ( !$this->add($row) ){
                return false;
            }
        }
        return true;
    }

    /**
     *  按条件删除记录
     *
     *  @param conditions 数组形式，查找条件，此参数的格式用法与find/findAll的查找条件参数是相同的。
     *
     */

    public function del($conditions)
    {
        $where = "";
        if (is_array($conditions))
        {
            $join = array();
            foreach( $conditions as $key => $condition )
            {
                $condition = $this->escape($condition);
                $join[] = "{$key} = {$condition}";
            }
            $where = "WHERE ( ".join(" AND ",$join). ")";
        }
        else
        {
            if(null != $conditions)$where = "WHERE ( ".$conditions. ")";
        }
        $sql = "DELETE FROM {$this->_tableName} {$where}";
        return $this->exec($sql);
    }
    

     /**
      * 按字段值修改一条记录
      *
      * @param conditions 数组形式，查找条件，此参数的格式用法与find / findAll的查找条件参数是相同的。
      * @param field 字符串，对应数据表中的需要修改的字段名
      * @param value 字符串，新值
      */

    public function updateField($conditions, $field, $value)
    {
        return $this->update($conditions, array($field=>$value));
    }

    


    /**
     * 修改数据，该函数将根据参数中设置的条件而更新表中数据
     *
     * @param conditions    数组形式，查找条件，此参数的格式用法与find/findAll的查找条件参数是相同的。
     * @param row    数组形式，修改的数据，
     * 此参数的格式用法与create的$row是相同的。在符合条件的记录中，将对$row设置的字段的数据进行修改。
     *
     */

    public function update($conditions, $row)
    {
        $where = "";
        //$row = $this->preFormat($row);
        if(empty($row))return FALSE;
        if(is_array($conditions)){
            $join = array();
            foreach( $conditions as $key => $condition ){
                $condition = $this->escape($condition);
                $join[] = "{$key} = {$condition}";
            }
            $where = "WHERE ".join(" AND ",$join);
        }else{
            if(null != $conditions)$where = "WHERE ".$conditions;
        }
        foreach($row as $key => $value){
            $value = $this->escape($value);
            $vals[] = "{$key} = {$value}";
        }
        $values = join(", ",$vals);
        $sql = "UPDATE {$this->_tableName} SET {$values} {$where}";
        return $this->exec($sql);
    }

    function __destruct() {
        @mysql_close($this->_con);
    }

}
?>
