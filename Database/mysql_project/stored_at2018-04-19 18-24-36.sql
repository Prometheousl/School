USE awlay;

CREATE TABLE `stored_at` (
  `SerialNum` int(11) NOT NULL,
  `LocID` int(11) NOT NULL,
  PRIMARY KEY (`SerialNum`,`LocID`),
  KEY `LocID` (`LocID`),
  CONSTRAINT `stored_at_ibfk_1` FOREIGN KEY (`SerialNum`) REFERENCES `equipment` (`SerialNum`),
  CONSTRAINT `stored_at_ibfk_2` FOREIGN KEY (`LocID`) REFERENCES `location` (`LocID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


