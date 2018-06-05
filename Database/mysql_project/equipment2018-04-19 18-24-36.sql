USE awlay;

CREATE TABLE `equipment` (
  `SerialNum` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  PRIMARY KEY (`SerialNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (1,'Phaser');
INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (2,'Badge');
INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (3,'Hypospray');
INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (4,'Uniform');
INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (5,'Spanner');
INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (6,'Drill');
INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (7,'Synthehol');
INSERT INTO `awlay`.`equipment`(`SerialNum`,`Name`) VALUES (8,'HoloEmitter');
