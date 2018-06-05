USE awlay;

CREATE TABLE `equipment_function` (
  `Function` varchar(255) NOT NULL,
  `SerialNum` int(11) NOT NULL,
  PRIMARY KEY (`Function`,`SerialNum`),
  KEY `SerialNum` (`SerialNum`),
  CONSTRAINT `equipment_function_ibfk_1` FOREIGN KEY (`SerialNum`) REFERENCES `equipment` (`SerialNum`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('2',1);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('weapon',1);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('1',2);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('5',2);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('6',2);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('communications',2);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('medical',3);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('8',4);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('clothing',4);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('3',5);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('engineering',5);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('engineering',6);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('4',7);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('entertainment',7);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('7',8);
INSERT INTO `awlay`.`equipment_function`(`Function`,`SerialNum`) VALUES ('special',8);
