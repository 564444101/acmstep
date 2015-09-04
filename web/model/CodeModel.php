<?php
class CodeModel extends Model{
    function __construct(){
        parent::__construct();
        $this->_tableName = 'code';
        $this->_tablePk = 'code_id';
    }       

    public function getByProbledmId($problemId = NULL){

        $conditions = array();
        $conditions []= array('problem_id' => $problemId);
        $sort = "run_time, run_memory, code_length";
        $list = array();
        $list = $this->findAll($conditions, $sort);
        return $list; 
    }
    
    public function getByUid($uid = NULL) {
        $conditions = array();
        $conditions['user_id'] = $uid;
        //$conditions []= array('user_id' => $uid);
        $sort = "code_id DESC";
        $list = array();
        $list = $this->findAll($conditions, $sort); 
        return $list;
    }

	public function getCodes($conditions){
		$sort = 'code_id DESC';		
		return $this->findAll($conditions, $sort);
	}

	public function getByTypeAndUid($type, $uid){
		$conditions = array();
		$conditions['user_id'] = $uid;
		$conditions['type'] = 0;
		return $this->findAll($conditions);
	}

    public function addCode($codeInfo){
		return $this->add($codeInfo);			
    }

	public function getCodeInfoByCid($cid){
		$codeInfo = array();
		$conditions['code_id'] = $cid;
		return $this->find($conditions);
	}

	public function updateCodeStatus($conditions, $status){
		$row = array();
		$row['judge_status'] = $status;
		return $this->update($conditions, $row);
	}
	public function getTopicSolvedCodes($uid){
		$fields = 'distinct problem_id';
		$conditions = 'judge_status=4 and user_id='.$uid;
		$sort = 'problem_id';		
		return $this->findAll($conditions,$sort,$fields);
	}
}   
?>
