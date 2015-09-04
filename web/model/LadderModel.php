<?php
	class LadderModel extends Model{
		function __construct(){
			parent::__construct();
			$this->_tableName = 'ladder';
			$this->_tablePk = 'ladder_id';
		}

		function addLadder($ladderInfo){
			$problemId = $ladderInfo['problem_id'];
			if ( $problemId < 1 || $problemId > 999 ){
				return false;
			}
			return $this->add($ladderInfo);
		}
	
		function getLadderList(){
			return $this->findAll(NULL,NULL,NULL);	
		}

		function getLadderListBeforeId($ladder_id){
			$condition = 'ladder_id<='.$ladder_id;
			return $this->findAll($condition, NULL, NULL);
		}

		function getLadderInfo($lid){
			$conditions = array();
			$conditions['ladder_id'] = $lid;
			return $this->find($conditions);
		}

		function getLadderInfoByPid($pid){
			$conditions = array();
			$conditions['problem_id'] = $pid;
			return $this->find($conditions);
		}

		function updateProblemId($lid,$pid){
			$conditions = array();
			$conditions['ladder_id'] = $lid;
			$field = 'problem_id';
			return $this->updateField($conditions, $field, $pid);	
		}

		function updateLadderInfo($oldLid, $ladderInfo){
			$conditions = array();
			$conditions['ladder_id'] = $oldLid;
			return $this->update($conditions, $ladderInfo);
		}

		function delLadder($lid){
			$conditions = array();
			$conditions['ladder_id'] = $lid;
			return $this->del($conditions);
		}

	}
?>
