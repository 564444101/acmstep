<?php
class LadderController extends Controller{
	function addLadderPage(){
		$this->checkAdmin();
		$this->display('addladderpage');
	}

	function addLadder(){
		$this->checkAdmin();

		$lid = $_POST['ladder_id'];
		$pid = $_POST['problem_id'];
		$ladderInfo = $_POST;

		$ladderModel = M('Ladder');	
		$problemModel = M('Problem');
			
		$problemInfo = $problemModel->getProblemInfo($pid);	
		if ( $pid >= 1000 ){
			$this->error('add ladder failed, the problem id has been bigger than 999');	
		}
		else if ( empty($problemInfo) ){
			$this->error('add ladder failed, the problem is not exist');	
		}
		else{
			$oldladderInfo = $ladderModel->getLadderInfoByPid($pid);
			if (!empty($oldladderInfo)){
			 $this->error('update ladder failed, the problem has been selected by other ladder');		
			}
			else{
				$oldladderInfo = $ladderModel->getLadderInfo($lid);
				if (!empty($oldladderInfo)){
					$this->error('add ladder info failed, the ladder id has existed');
				}
				if ($ladderModel->addLadder($ladderInfo)){
					$this->success('add ladder success!',U('Ladder','showLadderList'.'&pid=1'));		
				}
				else{
					$this->error('update failed, system error');
				}	
			}
		}
	}
	
	function showLadderList(){
		$this->checkAdmin();
		//$pid = $_GET['pid'];
		$ladderModel = M('Ladder');
		$ladderRows = $ladderModel->getLadderList();	
		//$ladderRows = $this->getPage($pid, 20, $ladderRows);

		if ( $ladderRows == false ){
			$this->error('get ladderlist failed', U('Index','index'));
		}
		else{
			$this->assign('ladderlist', $ladderRows);
			$this->display('ladderlist');
		}
	}

	function editLadder(){
		$this->checkAdmin();
		$lid = $_GET['lid'];
		$ladderModel = M('Ladder');
		$ladderInfo = $ladderModel->getLadderInfo($lid);
		
		if ( empty($ladderInfo) ){
			$this->error('get ladderinfo failed',U('Ladder','showLadderList').'&pid=1');
		}
		else{
			$this->assign('ladderinfo',$ladderInfo);
			$this->display('editladder');
		}
	}	

	/*
	 * 更新天梯关卡的题目
	 * 题目不能和休闲关卡的题目重复
	 * 题号范围在1~999之间
	 * 题号不能与已有的其他天梯关卡的题目重复
	 * 题号对应的题目必须是存在的
	 */
	function updateLadder(){

		$this->checkAdmin();

		$newLadderInfo = array();
			
		$oldLid = $_POST['old_ladder_id'];
		$lid = $_POST['ladder_id'];
		$pid = $_POST['problem_id'];
		$newLadderInfo['ladder_id'] = $lid;
		$newLadderInfo['problem_id'] = $pid;
		$newLadderInfo['ladder_name'] = $_POST['ladder_name'];

		$ladderModel = M('Ladder');	
		$problemModel = M('Problem');
			
		$problemInfo = $problemModel->getProblemInfo($pid);	
		if ( $pid >= 1000 ){
			$this->error('update ladder failed, the problem id has been bigger than 999');	
		}
		else if ( empty($problemInfo) ){
			$this->error('update ladder failed, the problem is not exist');	
		}
		else{
			$ladderInfo = $ladderModel->getLadderInfoByPid($pid);
			if ( !empty($ladderInfo) && $ladderInfo['ladder_id'] != $oldLid ){
			 $this->error('update ladder failed, the problem has been selected by other ladder');		
			}
			else{
				$ladderInfo = $ladderModel->getLadderInfo($lid);
				if (!empty($ladderInfo) && $ladderInfo['ladder_id'] != $oldLid){
					$this->error('update ladder info failed, the ladder id has existed');
				}
				if ($ladderModel->updateLadderInfo($oldLid, $newLadderInfo)){
					$this->success('update success!',U('Ladder','showLadderList'.'&pid=1'));		
				}
				else{
					$this->error('update failed, system error');
				}	
			}
		}
	}
		
	function delLadderProblem(){
		$this->checkAdmin();
		$lid = $_GET['lid'];
		$ladderModel = M('Ladder');		
		if ( $ladderModel->updateProblemId($lid, NULL) ){
			$this->success('del problem success!');
		}
		else{
			$this->error('del problem failed!');
		}
	}
	
	function delLadder(){
		$this->checkAdmin();
		$lid = $_GET['lid'];
		$ladderModel = M('Ladder');			
		if ( $ladderModel->delLadder($lid) ){
			$this->success('del ladder success',U('Ladder','showLadderList'.'&pid=1'));
		}	
		else{
			$this->error('del Problem failed',U('Ladder','showLadderList'.'&pid=1'));
		}
	}

}
?>
