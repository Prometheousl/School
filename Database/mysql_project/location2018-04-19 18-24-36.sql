USE awlay;

CREATE TABLE `location` (
  `LocID` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  PRIMARY KEY (`LocID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (1,'Bridge');
INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (2,'Engineering');
INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (3,'Transporter2');
INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (4,'Holodeck');
INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (5,'Medical');
INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (6,'Transporter1');
INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (7,'Quarters');
INSERT INTO `awlay`.`location`(`LocID`,`Name`) VALUES (8,'Ten Forward');
