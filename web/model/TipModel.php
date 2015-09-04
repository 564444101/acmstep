<?php
class TipModel extends Model{
    function __construct(){
        parent::__construct();
        $this->_tableName = 'tip';
        $this->_tablePk = 'user_id,problem_id';
    }       

	public function getTipInfo($uid, $pid){
		$conditions = array();
		$conditions['user_id'] = $uid;
		$conditions['problem_id'] = $pid;
		return $this->find($conditions);
	}
}   
?>
