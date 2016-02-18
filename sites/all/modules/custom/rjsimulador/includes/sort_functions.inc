<?php
function orderUserByUidASC($user1, $user2) {
  if ($user1->uid == $user2->uid) {
    return 0;
  }
  return ($user1->uid < $user2->uid) ? -1 : 1;
}

function orderUserByUidDESC($user1, $user2) {
  if ($user1->uid == $user2->uid) {
    return 0;
  }
  return ($user1->uid > $user2->uid) ? -1 : 1;
}

function orderUserByNameASC($user1, $user2) {
  if ($user1->name == $user2->name) {
    return 0;
  }
  return ($user1->name < $user2->name) ? -1 : 1;
}

function orderUserByNameDESC($user1, $user2) {
  if ($user1->name == $user2->name) {
    return 0;
  }
  return ($user1->name > $user2->name) ? -1 : 1;
}

function orderUserByCreatedASC($user1, $user2) {
  if ($user1->created == $user2->created) {
    return 0;
  }
  return ($user1->created < $user2->created) ? -1 : 1;
}

function orderUserByCreatedDESC($user1, $user2) {
  if ($user1->created == $user2->created) {
    return 0;
  }
  return ($user1->created > $user2->created) ? -1 : 1;
}

function orderUserByLasLoginASC($user1, $user2) {
  if ($user1->login == $user2->login) {
    return 0;
  }
  return ($user1->login < $user2->login) ? -1 : 1;
}

function orderUserByLastLoginDESC($user1, $user2) {
  if ($user1->login == $user2->login) {
    return 0;
  }
  return ($user1->login > $user2->login) ? -1 : 1;
}

function orderUserByLastAccessASC($user1, $user2) {
  if ($user1->login == $user2->login) {
    return 0;
  }
  return ($user1->login < $user2->login) ? -1 : 1;
}

function orderUserByLastAccessDESC($user1, $user2) {
  if ($user1->access == $user2->access) {
    return 0;
  }
  return ($user1->access > $user2->access) ? -1 : 1;
}