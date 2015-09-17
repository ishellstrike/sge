USE sge;

INSERT INTO `resource` (`name`) VALUES
    ('ore'),
	('cat'),
	('penguin'),
    ('lax'),
	('whale'),
	('ostrich');
	
INSERT INTO `player` (`id`, `name`, `password`, `registered`) VALUES
    (0, 'test', UNHEX('A94A8FE5CCB19BA61C4C0873D391E987982fBBD3'), '2015-01-22 11:12:13');
	
INSERT INTO `pilot` (`id`, `name`, `id_player`, `created`) VALUES
    (0, 'test_pilot', 0, '2015-01-22 11:13:14');