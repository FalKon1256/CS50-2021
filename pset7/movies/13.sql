-- In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.

SELECT people.name
FROM (SELECT * FROM people WHERE name = "Kevin Bacon" AND birth = 1958) AS p
LEFT JOIN (SELECT * FROM stars WHERE stars.person_id IN (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958)) AS s ON (p.id = s.person_id)
LEFT JOIN stars ON (s.movie_id = stars.movie_id)
LEFT JOIN people ON (stars.person_id = people.id)
WHERE
    people.id != (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958)
GROUP BY
    people.id