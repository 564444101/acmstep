<?php
	class ProblemController extends Controller {
		public function addProblemPage() {
			$this->checkAdmin();
			$this->display('addproblempage');
		}

		public function addProblem() {
			$this->checkAdmin();
			$problemModel = M('Problem');

			$problemInfo = $_POST;
			$problemInfo['solve_times'] = 0;
			$problemInfo['submit_times'] = 0;
		
			$input_data = $_FILE['inputfile'];
			if (!empty($input_data['tmp_name'])){
				if (is_uploaded_file($input_data['tmp_name'])){
					$problem_id = $problemInfo['problem_id'];
					$path = "data/".$problem_id."/".$problem_id."in";
					if (!move_uploaded_file($input_data['tmp_name'],$path)){
						$this->error('upload input data failed');
					}
				} else {
					$this->error('upload input data failed');
				}
			}

			$output_data = $_FILE['outputfile'];
			if (!empty($output_data['tmp_name'])){
				if (is_uploaded_file($output_data['tmp_name'])){
					$problem_id = $problemInfo['problem_id'];
					$path = "data/".$problem_id."/".$problem_id."out";
					if (!move_uploaded_file($output_data['tmp_name'],$path)){
						$this->error('upload output data failed');
					}
				} else {
					$this->error('upload output data failed');
				}
			}

			$pid = $problemModel->addProblem($problemInfo);
			if($pid === FALSE) {
				$this->error('add problem failed', U('Problem', 'showProblemList').'&pid=1');
			}
			else {
				$this->success('add problem success', U('Problem', 'showProblemList').'&pid=1'); 
			}
		}

		public function delProblem() {
			$this->checkAdmin();
			$problemModel = M('Problem');
			$pid = $_GET['pid'];;
			$status = $problemModel->delProblem($pid);
			if($status === FALSE) {
				$this->error('del problem failed', U('Problem', 'showProblemList').'&pid=1');
			}
			else {
				$this->success('del problem success', U('Problem', 'showProblemList').'&pid=1'); 
			}
		}

		public function updateProblem() {
			$this->checkAdmin();
			$problemModel = M('Problem');

			$old_id = $_POST['old_id'];
			unset($_POST['old_id']);

			$problemInfo = $_POST;

			$input_data = $_FILES['inputfile'];
			if (!empty($input_data['tmp_name'])){
				if (is_uploaded_file($input_data['tmp_name'])){
					$problem_id = $problemInfo['problem_id'];
					$dir_path = "data/".$problem_id;
					$path = $dir_path."/".$problem_id.".in";
					if (!is_dir($dir_path))	
						mkdir($dir_path);
					if (!move_uploaded_file($input_data['tmp_name'],$path)){
						$this->error('upload input data failed');
					}
				} else {
					$this->error('upload input data failed');
				}
			}

			$output_data = $_FILES['outputfile'];
			if (!empty($output_data['tmp_name'])){
				if (is_uploaded_file($output_data['tmp_name'])){
					$problem_id = $problemInfo['problem_id'];
					$path = "data/".$problem_id."/".$problem_id.".out";
					if (!move_uploaded_file($output_data['tmp_name'],$path)){
						$this->error('upload output data failed');
					}
				} else {
					$this->error('upload output data failed');
				}
			}
			unset($problemInfo['inputfile']);
			unset($problemInfo['outputfile']);

			$status = $problemModel->updateProblem($old_id, $problemInfo);

			if($status === FALSE) {
				$this->error('update problem failed', U('Problem', 'showProblem')."&pid={$problemInfo['problem_id']}");
			}
			else {
				$this->success('update problem success', U('Problem', 'showProblem')."&pid={$problemInfo['problem_id']}"); 
			}
		}

		public function showProblem() {
			$this->checkAdmin();
			$pid = $_GET['pid'];;
			$problemModel = M('Problem');
			$problemInfo = $problemModel->getProblemInfo($pid);
			if($problemInfo === FALSE) {
				$this->error('show problem failed', U('Problem', 'showProblemList').'&pid=1');
			}
			else {
				$this->assign('probleminfo', $problemInfo);
				$this->display('editproblem');
			}
		}

		public function showProblemList() {
			$this->checkAdmin();
			$pid = $_GET['pid'];;
			$problemModel = M('Problem');
			$problemRows = $problemModel->getProblemList();
			$problemRows = $this->getPage($pid, 20, $problemRows); 
			if($problemRows === FALSE) {
				$this->error('show problem list failed', U('Problem', 'showProblemList').'&pid=1');
			}
			else {
				$this->assign('problemList', $problemRows['rows']);
				$this->display('showproblemlist');
			}
		}
	}
?>
