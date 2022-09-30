-- Keep a log of any SQL queries you execute as you solve the mystery.


--  Why: Need to acquire more detail info, in fact we only know when and where the theft took place.
-- Info: Find clues from descriptions of crime_scene_reports.
SELECT
    description
FROM crime_scene_reports
WHERE
    year = 2020 AND
    month = 7 AND
    day = 28 AND
    street = "Chamberlin Street";


--  Why: Thanks to the descriptions, we now have the clues of some key words as follows:
--       1. "10:15am"  2. "courthouse"  3."Interviews"  4. "three witnesses".
-- Info: Get the interview transcripts from three witnesses
SELECT
    id, name, transcript
FROM interviews
WHERE
    year = 2020 AND
    month = 7 AND
    day = 28 AND
    transcript LIKE "%courthouse%";


--  Why: From Ruth, one of the witnesses, we know that the thief left the courthouse in 10:15 AM ~ 10:25 AM.
--       It's possible to get the next clue investigating courthouse security logs.
-- Info: Find suspects from tables as follows: courthouse_security_logs and people tables.
SELECT
    people.id, name
FROM courthouse_security_logs
LEFT JOIN people ON (courthouse_security_logs.license_plate = people.license_plate)
WHERE
    year = 2020 AND
    month = 7 AND
    day = 28 AND
    people.license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE month = 7 AND day = 28 AND activity = "entrance" AND CASE
                                                                                                                                        WHEN hour < 10 THEN minute <= 60
                                                                                                                                        WHEN hour = 10 THEN minute <= 15
                                                                                                                                        END
                     ) AND
    hour = 10 AND
    minute BETWEEN 15 AND 25 AND
    activity = "exit"
ORDER BY
    hour, minute;


--  Why: From Eugene, one of the witnesses, we believe that the thief withdrawed money by the ATM on Fifer Street on July 28.
-- Info: Find suspects tables as follows: atm_transactions, bank_accounts, and people tables.
SELECT
    people.id, name
FROM atm_transactions
LEFT JOIN bank_accounts ON (atm_transactions.account_number = bank_accounts.account_number)
LEFT JOIN people ON (bank_accounts.person_id = people.id)
WHERE
    month = 7 AND
    day = 28 AND
    atm_location = "Fifer Street" AND
    transaction_type = "withdraw";


--  Why: From Raymond, one of the witnesses, we know that the thief made a phone call to his accomplice (less than a minute),
--       seems asking to purchase the flight ticket for him, and planning to take the earliest flight out of Fiftyville on July 29.
-- Info: Find the suspects of the thief and his accomplice from phone_calls and people table.
SELECT
    c.id, c.name, r.id, r.name
FROM phone_calls
LEFT JOIN people AS c ON (phone_calls.caller = c.phone_number)
LEFT JOIN people AS r ON (phone_calls.receiver = r.phone_number)
WHERE
    month = 7 AND
    day = 28 AND
    duration < 60;


--  Why: We've now have three lists of suspects who could be the thief. To narrow the suspect list down, we need to take intersection of three lists.
-- Info: Maintain a list of suspects.
SELECT
    people.id, name
FROM courthouse_security_logs
LEFT JOIN people ON (courthouse_security_logs.license_plate = people.license_plate)
WHERE
    year = 2020 AND
    month = 7 AND
    day = 28 AND
    people.license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE month = 7 AND day = 28 AND activity = "entrance" AND CASE
                                                                                                                                        WHEN hour < 10 THEN minute <= 60
                                                                                                                                        WHEN hour = 10 THEN minute <= 15
                                                                                                                                        END
                     ) AND
    hour = 10 AND
    minute BETWEEN 15 AND 25 AND
    activity = "exit"

INTERSECT

SELECT
    people.id, name
FROM atm_transactions
LEFT JOIN bank_accounts ON (atm_transactions.account_number = bank_accounts.account_number)
LEFT JOIN people ON (bank_accounts.person_id = people.id)
WHERE
    month = 7 AND
    day = 28 AND
    atm_location = "Fifer Street" AND
    transaction_type = "withdraw"

INTERSECT

SELECT
    c.id, c.name
FROM phone_calls
LEFT JOIN people AS c ON (phone_calls.caller = c.phone_number)
LEFT JOIN people AS r ON (phone_calls.receiver = r.phone_number)
WHERE
    month = 7 AND
    day = 28 AND
    duration < 60;


--  Why: Due to the the flight ticket that the thief has purchased, we can identify who is the thief now.
-- Info: By comparing the flight time, we'll get to the bottom of who the thief is.
SELECT
    people.id, name, hour, minute, o.city, d.city
FROM passengers
LEFT JOIN flights ON (passengers.flight_id = flights.id)
LEFT JOIN people ON (passengers.passport_number = people.passport_number)
LEFT JOIN airports AS o ON (flights.origin_airport_id = o.id)
LEFT JOIN airports AS d ON (flights.destination_airport_id = d.id)
WHERE
    year = 2020 AND
    month = 7 AND
    day = 29 AND
    people.id IN ("514354", "686048")
ORDER BY
    hour;
    
    
-- Conclusion: The thief is Ernest, and his accomplice is Berthold.