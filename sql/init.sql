USE `sge`;
DROP TABLE IF EXISTS `market`;
DROP TABLE IF EXISTS `resource`;
DROP TABLE IF EXISTS `item`;
DROP TABLE IF EXISTS `pilot`;
DROP TABLE IF EXISTS `player`;

-- CREATE PART --

CREATE TABLE `player` (
	`id` INT NOT NULL AUTO_INCREMENT,
	PRIMARY KEY (`id`),
	
	`name` CHAR(30) NOT NULL,
	`password` BINARY(20) NOT NULL,
	
	`registered` DATETIME NOT NULL
);

CREATE TABLE `pilot` (
	`id` INT NOT NULL AUTO_INCREMENT,
	PRIMARY KEY (`id`),
	
	`name` CHAR(30) NOT NULL,
	
	`id_player` INT NOT NULL,
	FOREIGN KEY (`id_player`) 
        REFERENCES `player`(`id`)
        ON DELETE CASCADE,
	
	`created` DATETIME NOT NULL
);

CREATE TABLE `resource` (
	`id` INT NOT NULL AUTO_INCREMENT,
	PRIMARY KEY (`id`),
	
	`name` CHAR(30) NOT NULL
);

CREATE TABLE `item` (
	`id` INT NOT NULL AUTO_INCREMENT,
	PRIMARY KEY (`id`),
	
	`name` CHAR(30) NOT NULL
);

CREATE TABLE `market` (
	`id` INT NOT NULL AUTO_INCREMENT,
	PRIMARY KEY (`id`),
	
	`id_item` INT NOT NULL,
	FOREIGN KEY (`id_item`) 
        REFERENCES `item`(`id`)
        ON DELETE CASCADE,
		
	`count` INT NOT NULL,
	
	`started` TIME NOT NULL
);

CREATE TABLE `market_history` (
	`id` INT NOT NULL AUTO_INCREMENT,
	PRIMARY KEY (id),
	
	`id_item` INT NOT NULL,
	FOREIGN KEY (`id_item`) 
        REFERENCES `item`(`id`)
        ON DELETE CASCADE,
		
	`is_complete` BOOLEAN NOT NULL,
	
	`finish` DATETIME NOT NULL
);