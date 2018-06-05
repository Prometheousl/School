USE awlay;

CREATE TABLE `department` (
  `DepNum` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `LocID` int(11) NOT NULL,
  PRIMARY KEY (`DepNum`),
  KEY `LocID` (`LocID`),
  CONSTRAINT `department_ibfk_1` FOREIGN KEY (`LocID`) REFERENCES `location` (`LocID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


INSERT INTO `awlay`.`department`(`DepNum`,`Name`,`LocID`) VALUES (1,'Command',1);
INSERT INTO `awlay`.`department`(`DepNum`,`Name`,`LocID`) VALUES (2,'Security',1);
INSERT INTO `awlay`.`department`(`DepNum`,`Name`,`LocID`) VALUES (3,'Science',8);
INSERT INTO `awlay`.`department`(`DepNum`,`Name`,`LocID`) VALUES (4,'Engineering',2);
INSERT INTO `awlay`.`department`(`DepNum`,`Name`,`LocID`) VALUES (5,'Medical',5);
INSERT INTO `awlay`.`department`(`DepNum`,`Name`,`LocID`) VALUES (6,'Operations',3);
INSERT INTO `awlay`.`department`(`DepNum`,`Name`,`LocID`) VALUES (7,'Expendable',7);
