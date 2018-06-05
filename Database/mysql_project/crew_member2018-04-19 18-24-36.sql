USE awlay;

CREATE TABLE `crew_member` (
  `SSN` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `Rank` varchar(255) NOT NULL,
  `Planet_of_Origin` varchar(255) NOT NULL,
  `LocID` int(11) NOT NULL,
  `DepNum` int(11) NOT NULL,
  PRIMARY KEY (`SSN`),
  KEY `LocID` (`LocID`),
  KEY `DepNum` (`DepNum`),
  CONSTRAINT `crew_member_ibfk_1` FOREIGN KEY (`LocID`) REFERENCES `location` (`LocID`),
  CONSTRAINT `crew_member_ibfk_2` FOREIGN KEY (`DepNum`) REFERENCES `department` (`DepNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (1,'Picard','Captain','Earth',1,1);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (2,'Reiker','Commander','Earth',1,1);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (3,'Data','Lieutenant Commander','Omicron Theta',8,6);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (4,'Worf','Lieutenant','Klingon',3,2);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (5,'Geordie','Chief of Engineering','Earth',2,4);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (6,'Troi','Advisor','Betazed',3,4);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (7,'Beverly','Chief Medical Officer','Earth',5,5);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (8,'O''Brien','Chief Petty Officer','Earth',6,7);
INSERT INTO `awlay`.`crew_member`(`SSN`,`Name`,`Rank`,`Planet_of_Origin`,`LocID`,`DepNum`) VALUES (9,'Wesley','Ensign','Earth',1,6);
