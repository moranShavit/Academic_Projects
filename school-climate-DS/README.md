# SISE2601 Project Data Description

## How much does school climate affect grades in core subjects in the education system?

### Dor Benita, Moran Shavit, Amit Lalush, Yoav Evron

------------------------------------------------------------------------

### Overview

Our project uses three datasets retrieved from the [RAMA "Meitzav" data portal](https://data.gov.il/dataset/rama_meitzav), containing information about school performance, school climate, and school background characteristics.

The datasets are: 1. Scores by schools – Annual "Meitzav" test results per school. 2. Climates by school – Responses to teacher and pupils surveys assessing school climate. 3. Metadata on schools – Metadata about each school’s sector, location, and socio-economic level.

These datasets were merged using school identifiers and test years, creating a unified dataset that allows for rich analysis of the connections between school climate and student performance.

The goal of this project is to explore how different dimensions of school climate relate specifically to *students' performance in mathematics*.\
To do so, we constructed several composite indices from dozens of survey items, capturing aspects such as exposure to violence, classroom discipline, emotional support, student satisfaction, and feedback from teachers.

Our main research question is:\
*Which climate-related factors are most strongly associated with higher math scores across schools?*

By applying data preprocessing, aggregation, and statistical modeling in R, we aim to identify the most influential climate indicators that explain variation in mathematics performance — and ultimately, provide insights that can support more effective educational environments.

------------------------------------------------------------------------

### Table 1: Scores by schools (results in R)

This dataset provides average "Meitzav" test scores for different subjects, per school and year.

| Variable Name | Description                                     | Type        |
|-------------------|----------------------------------|-------------------|
| year        | Year of the "Meitzav" test                      | Numeric     |
| semel_mosad | Unique school identifier                        | Numeric     |
| shichva     | Grade level tested (e.g., 5 or 8)               | Numeric     |
| subject_id  | Subject code: H (Hebrew), M (Math), E (English) | Categorical |
| score       | Average test score for the subject              | Numeric     |

------------------------------------------------------------------------

### Table 2: Climates by school (atmosphere in R)

This dataset contains survey results from teachers and pupils in various schools, measuring aspects of school climate.

Each row’s value field reflects the school's score (from 1 to 100) on a specific climate item, calculated from the percentage of students who agreed with the statement.

| Variable Name | Description | Type |
|-------------------|----------------------------------|-------------------|
| year | Year of the survey | Numeric |
| semel_mosad | Unique school identifier | Numeric |
| shichva | Grade level group | Categorical |
| var_id | Variable code used to identify a specific question | Categorical |
| var_name | Description of the question in Hebrew | Text |
| value | Survey result score | Numeric |
| index_id | Optional grouping code for related variables | Text |

## Features: Violence

This index reflects students' self-reported experiences of physical and verbal violence at school, including bullying and social exclusion.

| var_id | Description |
|----------------------|--------------------------------------------------|
| T_1032_Di | In the past month, I was punched, kicked, or hit by another student |
| T_1033_Di | In the past month, a student threatened me to give money, food, or valuables |
| T_1034_Di | In the past month, a student hit me hard |
| T_1035_Di | In the past month, a student pushed me |
| T_1036_Di | In the past month, a student threatened to hurt me |
| T_1037_Di | In the past month, a student used an object to hurt me |
| T_1038_Di | In the past month, a student tried to get others not to talk to me |
| T_1039_Di | In the past month, a student spread false rumors about me |
| T_1040_Di | In the past month, I was socially excluded (boycotted) |
| T_Violence | Composite index: Student involvement in violent events |

### Features: Anti-Violence Efforts

This index captures students’ perceptions of the school's proactive efforts to prevent and respond to violence.

| var_id | Description |
|----------------------|--------------------------------------------------|
| T_1009_Di | The school takes many actions to prevent and address violence |
| T_1013_Di | There is always a teacher on duty during breaks to monitor for violence |
| T_1016_Di | Teachers are aware when violence occurs at school |
| T_EffortViolence | Composite index: Students’ perception of school efforts to handle violence |

### Features: Caring

This index represents students’ feelings of emotional support and caring relationships with teachers.

| var_id | Description |
|---------------------|---------------------------------------------------|
| T_1003_Di | I have good and close relationships with most of my teachers |
| T_1007_Di | When I feel sad or upset, I feel comfortable talking to one of my teachers |
| T_1011_Di | Teachers care about me and not only about my studies |
| T_1018_Di | It is important to the teachers to know how I feel at school and in general |
| T_Caring | Composite index: Students’ sense of emotional support and teacher caring |

### Features: Atmosphere

This index measures classroom discipline, mutual respect, and behavioral norms as perceived by students.

| var_id | Description |
|---------------------|---------------------------------------------------|
| T_1001_Di | Students in my class treat teachers with respect |
| T_1010_Rd | Students rarely make noise and disturb the class |
| T_1012_Rd | There are no students in my class who are rude to teachers |
| T_1030_Rd | Teachers don’t have to wait long at the beginning of class for silence |
| T_Atmosphere | Composite index: Classroom discipline and student behavior |

### Features: Satisfaction

This index reflects students’ overall sense of satisfaction, belonging, and comfort at school.

| var_id | Description |
|---------------------|---------------------------------------------------|
| T_1005_Rd | Even if I could, I would not transfer to another school |
| T_1017_Di | I feel good at school |
| T_1056_Di | I enjoy being at school |
| T_Satisfaction | Composite index: Students’ general satisfaction and sense of belonging |

### Features: Feedback

This index assesses how much personalized support and feedback students feel they receive from teachers.

| var_id | Description |
|---------------------|---------------------------------------------------|
| T_1014_Di | Most teachers keep me updated about my academic progress |
| T_1050_Di | Most teachers explain to each student personally what they need to improve |
| T_1053_Di | Teachers write comments explaining what is right and what needs improvement |
| T_1054_Di | Teachers help students understand why they didn’t succeed in tests or work |
| T_1055_Di | Teachers provide ongoing feedback about learning |
| T_1059_Di | Teachers ensure students understand before moving on to the next topic |
| T_Feedback | Composite index: Students’ perception of teacher feedback and support |

------------------------------------------------------------------------

### Table 3: Metadata on schools (background in R)

This dataset includes metadata about each school, such as name, location, and socio-economic ranking.

| Variable Name | Description | Type |
|-------------------|----------------------------------|-------------------|
| semel_mosad | Unique school identifier | Numeric |
| shem_mosad | School name | Text |
| survey_group | Survey Religious and ethnic group | Numeric |
| pikuach | Supervision type | Numeric |
| migzar | Sector (e.g., Jewish, Arab, etc.) | Numeric |
| rashut | Local municipality code | Numeric |
| rashut_teur | Name of local municipality | Text |
| ses_mosad_cat_yh | Socio-economic index category (Jewish schools) | Categorical |
| ses_mosad_cat_e | Socio-economic index category (Arab schools, if available) | Categorical |

This table helps understand differences in school achievement and climate by including geographic and demographic factors.

------------------------------------------------------------------------

### Notes

-   The datasets were joined using the semel_mosad (school identifier) and year columns.
-   Variables in the climate data were pivoted to wide format using var_id as column names and value as values.