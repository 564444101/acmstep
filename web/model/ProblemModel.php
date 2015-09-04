<?php
class ProblemModel extends Model{
	function __construct(){
		parent::__construct();
		$this->_tableName = 'problem';
		$this->_tablePk = 'problem_id';
	}
	

	function getProblemInfo($problem_id){

		$field = 'problem_id';
		$problemInfo = $this->findBy($field, $problem_id);		
		return $problemInfo;
	}
	
	function getProblemNameById($problem_id){
		$conditions = array();
		$conditions['problem_id'] = $problem_id;
		$fields = 'problem_name';
		$res = $this->find($conditions, NULL, $fields);
		if (empty($res)){
			return NULL;
		}
		else{
			return $res['problem_name'];
		}
	}

	function getProblemList(){

		return $this->findAll();	
	}	

	function addProblem($problemInfo){
		
		return $this->add($problemInfo);
	}

	function updateData($problemId, $data){

		$conditions = array();	
		$conditions['problem_id'] = $problemId;

		$row = array();		
		$row['input_data_path'] = $data['input_data_path'];
		$row['output_data_path'] = $data['output_data_path'];
		return $this->update($condtions, $row);
	}

	function updateProblemStatistic($problemId, $row){

		if ( NULL == $problemId ) {
			return false;
		}

		$conditions = array(); 
		$conditions['problem_id'] = $problemId;
		$statistic = array();
		if ( NULL != $row['solve_times'] )
			$statistic['solve_times'] = $row['solve_times'];
		if ( NULL != $row['submit_times'] )
			$statistic['submit_times'] = $row['submit_times'];

		return $this->update($conditions, $statistic);
	}
	
	function updateProblem($pid, $problemInfo){
		
		$conditions = array();
		$conditions['problem_id'] = $pid;

		return $this->update($conditions, $problemInfo);
	}
	
	function delProblem($problem_id){

		$conditions = array();
		$conditions['problem_id'] = $problem_id;
		return $this->del($conditions);
	}
}

?>
