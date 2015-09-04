<?php
	class UserModel extends Model{
		function __construct() {
			parent::__construct();
			$this->_tableName = 'user';
			$this->_tablePk = 'user_id';
		}
	
		function getUserInfo($uid){

			$field = 'user_id';
		    $rows = $this->findBy($field, $uid);	
			return $rows;
		}

		function getUserByUsername($username){
			$field = 'username';
			return $this->findBy($field, $username);
		}

		function getPassWdByUsername($username){
			$conditions = array();
			$conditions['username'] = $username;
			$fields = 'password';
			return $this->find($conditions, NULL, $fields);
		}
		
		function getAllUsers(){
				
			$sort = "user_id DESC";
			$rows = $this->findAll(NULL, $sort);
			return $rows;
		}

		function addUser($userInfo){

			if ( NULL == $userInfo['username'] || NULL == $userInfo['password'] ){
				return false;
			}

			return $this->add($userInfo);
		}

		function updateUserState($userId, $field, $value){

			$conditions = array();
			$conditions['user_id'] = $userId;
			return $this->updateField($conditions, $field, $value);	
		}

		function updateUserInfo($uid, $userInfo){
				
			$conditions = array();
			$conditions['user_id'] = $uid;
			return $this->update($conditions, $userInfo);
		}

		function delUser($uid){

			$conditions = array();
			$conditions['user_id'] = $uid;		
			return $this->del($conditions);
		}

		function getUserListByTopicRank(){
			$sort = 'topic_rank ASC';	
			return $this->findAll(NULL, $sort, NULL, NULL);
		}
		
		function getUserListByLadderRank(){
			$sort = 'ladder_rank ASC';	
			return $this->findAll(NULL, $sort, NULL, NULL);
		}
	}

?>
