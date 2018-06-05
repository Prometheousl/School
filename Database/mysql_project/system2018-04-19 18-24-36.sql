USE awlay;

CREATE TABLE `system` (
  `ID` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `Status` varchar(255) NOT NULL,
  `DepNum` int(11) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `DepNum` (`DepNum`),
  CONSTRAINT `system_ibfk_1` FOREIGN KEY (`DepNum`) REFERENCES `department` (`DepNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


INSERT INTO `awlay`.`system`(`ID`,`Name`,`Status`,`DepNum`) VALUES (1,'Life','Good',5);
INSERT INTO `awlay`.`system`(`ID`,`Name`,`Status`,`DepNum`) VALUES (2,'Phasers','Warning',2);
INSERT INTO `awlay`.`system`(`ID`,`Name`,`Status`,`DepNum`) VALUES (3,'Shields','Bad',2);
INSERT INTO `awlay`.`system`(`ID`,`Name`,`Status`,`DepNum`) VALUES (4,'Warp Core','Warning',4);
INSERT INTO `awlay`.`system`(`ID`,`Name`,`Status`,`DepNum`) VALUES (5,'Photon Torpedos','Good',2);
INSERT INTO `awlay`.`system`(`ID`,`Name`,`Status`,`DepNum`) VALUES (6,'Transporters','Warning',6);
INSERT INTO `awlay`.`system`(`ID`,`Name`,`Status`,`DepNum`) VALUES (7,'Communications','Bad',3);
