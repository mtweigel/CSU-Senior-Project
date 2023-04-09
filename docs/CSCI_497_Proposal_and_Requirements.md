![CSU Logo](https://github.com/mtweigel/CSU-Senior-Project/raw/master/media/CSU_logo.svg?raw=true)

CSCI 497 Senior Project Design
==============================

### Proposal

- [ ] Name: Matthew Weigel :man:

- [ ] Degree/Major: Bachelor of Science, Computer Science :computer:

- [ ] Project Advisor: Dr. Sean Hayes :older_man:

- [ ] Expected Graduation: Fall 2023 :date:

- [ ] Problem Statement :bulb:
- Ideally, a video editing program is both user-friendly and intuitive, perfect for anyone who wants to create video projects. Unfortunately, many existing video editing programs are both cumbersome and unintuitive, making it difficult for casual users to make simple edits to their video clips. This is a problem for people such as educators or content-creators, who need to edit video recordings as part of their job. The proposed solution to this problem is to develop a simplified video editing software program with a user-friendly interface, streamlined features, and easy accessibility for casual users. This solution benefits the people who are not video editing enthusiasts, but who want to make easy edits to their video clips without having to spend time learning complex software. In summary, the problem of complex and cumbersome video editing software can be solved by developing software that is more accessible to a wider range of users by simplifying the video editing process, making it less time-consuming and more enjoyable for everyone.

- [ ] Project Description :memo:
- This project will be a free video editing desktop application that focuses on simplicity and usability. Users will have access to a number of common features such as video trimming, intro and outro creation, and audio dubbing, all contained in a user-friendly GUI.

- [ ] Proposed Implementation Language(s) :octocat:
- This project will be implemented using C++ and Python.

- [ ] Libraries, Packages, Development Kits, etc. to be used in in the proposed implementation languages(s) :wrench:
- I will be using either Qt or Electron JS as frameworks to build my desktop application.

- [ ] Additional Software/Equipment Needed :computer::joystick:
- The only equipment needed is a PC running Windows.

- [ ] Personal Motivation :cake:  
- This project is highly motivated by my personal experiences with the issues described in my problem statement. As a result, I have a strong interest in using this application myself. Additionally, this project presents a significant opportunity for me to expand my knowledge and experience in the field of computer science. By utilizing new frameworks to build my most complex program yet, I will gain valuable skills and insights that will benefit me both personally and professionally.

- [ ] Outline of Future Research Efforts :books::rocket:  
- I plan to complete the project in steps, starting with research that will include using resources such as YouTube to learn how to use the Qt framework and W3 Schools to brush up on C++ and Python. Next, I will finalize the design using mockups and prototypes. After that, the efforts to build the application will begin until it is finished. Finally, I will do thorough testing both during and after development.

- [ ] Schedule :calendar: :watch:  
- The scheduled plan is to complete this project over the summer and have it finished by the fall semester. Below is a proposed schedule based on my outline of future research efforts.
	- Week 1 (5/6-5/13): Research frameworks and get a preliminary feel for the scope of the project. 
	- Week 2 (5/13-5/20): Write the test plan.
	- Weeks 3-4 (5/20-6/3): Create mockups then build GUI.
	- Weeks 5-8 (6/3-7/1): Implement app features.
	- Weeks 9-10 (7/1-7/15): Do thorough testing.
	- Weeks 11-12 (7/15-7/29): Finalize project for delivery.
	
	
### Requirements Document

- **ID#:** 1 **Type:** Functional 
**Description:** The video editing application will be kept simplified, but will still contain many common features included but not limited to: video import and export, scrubbing, trimming, intro and outro creation, and audio dubbing. 
**Rationale:** The average user should not feel overwhelmed by the number of features, but should still be satisfied by the features available.
**Fit Criterion:** The user is content with the available features and can effortlessly perform basic editing tasks.
**Priority:** This is of highest priority for the project.
**Dependencies:** None.

- **ID#:** 2 **Type:** Appearance
**Description:** The graphical user-interface of the application should be visually attractive to the user by utilizing pleasing colors, fonts, and icons.
**Rationale:** A visually pleasing user-interface plays a crucial role in ensuring a positive user experience.
**Fit Criterion:** Users should report that the user-interface is visually pleasing.
**Priority:** Appearance is of medium priority, behind functionality.
**Dependencies:** ID# 3.

- **ID#:** 3 **Type:** Style
**Description:** The style and layout of the graphical user-interface should be consistent with the standards set by other video editing programs, while still maintaining originality.
**Rationale:** New users need to be able to quickly identify features that they expect from a video editing application.
**Fit Criterion:** A new user is able to quickly find important features such as video file importing.
**Priority:** A consistent, recognizable style and layout is important but not of high priority.
**Dependencies:** ID# 1.

- **ID#:** 4 **Type:** Ease of Use
**Description:** One of the main goals of the project is to make the application easy to use for all users. This includes making the the application easy to remember, and minimizing the potential for user errors.
**Rationale:** Ease of use boosts user confidence, which increases the likelihood that the user will want to use the application again.
**Fit Criterion:** A user who has never edited before is able to comfortably and confidently use the application and all of its features.
**Priority:** This is of high priority because it is one of the central goals.
**Dependencies:** ID# 3.

- **ID#:** 5 **Type:** Personalization
-**Description:** This involves allowing the user to customize the layout and appearance of the application to match their preference.
**Rationale:** Incorporating elements of personalization is an important part of ensuring user satisfaction.
**Fit Criterion:** The user is able to make minor changes to personalize the application.
**Priority:** This is of lower priority.
**Dependencies:** ID# 1.

- **ID#:** 6 **Type:** Learning
-**Description:** There will be elements such as tooltips and contextual menus that will help facilitate easy learning of the application in a short amount of time. These will be available for users who need them, without being imposed on those who do not.
**Rationale:** It should be easy for new users to learn to edit videos on the application.
**Fit Criterion:** A new user is able to quickly learn how to use the features provided within the application.
**Priority:** This is of higher priority because it coincides with ease of use.
**Dependencies:** ID# 4.

- **ID#:** 7 **Type:** Understandability
**Description:** To enhance understandability, the application's processes will aim to minimize the use of lengthy text and rely more on short words and symbols where appropriate. While language translation would be highly advantageous, it is unlikely to be included in this project.
**Rationale:** Allowing people of all languages to understand the application increases its potential userbase.
**Fit Criterion:** Someone who cannot read or does not speak English is able to understand the application with little to no issue.
**Priority:** This is of low priority because most of this application's users will speak English.
**Dependencies:** ID# 5.

- **ID#:** 8 **Type:** Accessibility
**Description:** The application should be accessible to all users, including those with physical, visual, hearing, and cognitive disabilities.
**Rationale:** Increased accessibility allows a wider range of people to use the application.
**Fit Criterion:** A user with disabilities is able to easily use the application.
**Priority:** Low priority.
**Dependencies:** ID# 4.

- **ID#:** 9 **Type:** Convenience
**Description:** There will be a large focus on simplicity and convenience. This can be done by minimizing the steps required to perform an specific edit to a video.
**Rationale:** Nowadays, people have become quite accustomed to convenience in everyday life, so it is important for this application to follow this trend so that users can more easily achieve their desired results without feeling frustrated by a complicated editing process.
**Fit Criterion:** A new user does not feel inconvenienced by the processes of the application.
**Priority:** High priority.
**Dependencies:** ID# 4.

- **ID#:** 10 **Type:** Speed and Latency
**Description:** The video editing application should perform editing actions with reasonable speed. This ensures that users can efficiently edit their videos without facing unnecessary delays or interruptions.
**Rationale:** In today's fast-paced world, it's important for video editing software to perform editing actions with reasonable speed to allow users to edit their videos without wasting time waiting for the software to complete tasks.
**Fit Criterion:** The user is satisfied with the time it takes for the application to process editing tasks.
**Priority:** The is of high priority because a slow program would greatly inhibit user satisfaction.
**Dependencies:** ID# 1.

- **ID#:** 11 **Type:** Precision or Accuracy
**Description:** Precision and accuracy will be emphasized in the editing processes of this application.
**Rationale:** For users to be satisfied with its performance, this application must exhibit both precision and accuracy in its editing processes so that the final product accurately reflects their creative vision.
**Fit Criterion:** The product of a user's edits accurately reflects what they expected.
**Priority:** High priority.
**Dependencies:** ID# 1.

- **ID#:** 12 **Type:** Reliability and Availability
**Description:** In order to achieve reliability and availability, this application will have measures in place to reduce the chances of crashing or file corruption in the case that it becomes overwhelmed. 
**Rationale:** Users need to be confident that large file sizes and edit requests will not cause the application to crash or error out.
**Fit Criterion:** The user is able to use the application without crashes or errors.
**Priority:** High priority.
**Dependencies:** ID# 9.

- **ID#:** 13 **Type:** Robustness or Fault-Tolerance
**Description:** In the case of a fatal error or crash, the application needs to be robust enough to be able to recover the user's editing progress.
**Rationale:** Failure to save a user's progress in the event of a crash could result in substantial dissatisfaction among users.
**Fit Criterion:** The user's progress is regularly auto-saved to prevent loss of progress in the event of a crash.
**Priority:** High priority.
**Dependencies:** ID# 12.

- **ID#:** 14 **Type:** Capacity
**Description:** The application should have a clear definition of its maximum capacity in terms of the number of clips, duration, file size, and resolution.
**Rationale:** By defining the maximum capacity of things like file size, it reduces the chance of the application crashing from being overloaded.
**Fit Criterion:** The user receives an on-screen message if the file they are importing is too large.
**Priority:** This is of high priority because it is large part of keeping the application reliable and available.
**Dependencies:** ID# 12.

- **ID#:** 15 **Type:** Productization
**Description:** This video editing application will be standalone executable that users will able to download directly without using an installer. In the future, an installer may be used.
**Rationale:** A standalone executable allows for the user to download the application without needing to involve an installer. It also lowers the complexity of the project.
**Fit Criterion:** The user is able to download the entire application directly.
**Priority:** High priority.
**Dependencies:** ID# 1.

- **ID#:** 16 **Type:** Supportability
**Description:** This application will provide support to users in the form of a help button that leads to easy-to-read documentation.
**Rationale:** Some users may want further help with navigating the site and exploring its features. This documentation will provide that.
**Fit Criterion:** A user who needs help is able to spot the help button and easily read and understand the support documentation.
**Priority:** This is of low-medium priority because the built in tooltips should be adequate in most cases.
**Dependencies:** ID# 6.

- **ID#:** 17 **Type:** Integrity
**Description:** The application will have appropriate measures set in place to prevent erroneous data or intentional abuse from being introduced which could harm it.
**Rationale:** By protecting the application from potential harm caused by incorrect data or malicious attacks, users can be assured of a safeguarded experience.
**Fit Criterion:** The user is unable to enter or import data that would cause fatal errors.
**Priority:** High priority.
**Dependencies:** ID# 12.

- **ID#:** 18 **Type:** Privacy
**Description:** The application will take measures to ensure that a user's videos are not stored longer than necessary for editing purposes. Once the user saves their video and closes the application, the program will remember where they left off but not store any data beyond that.
**Rationale:** The user will expect that when they import their videos for editing, the videos will not be stored beyond what is necessary because that would violate their privacy.
**Fit Criterion:** The user's video is not stored beyond what is necessary for their own editing convenience.
**Priority:** High priority.
**Dependencies:** ID# 9.

- **ID#:** 19 **Type:** Immunity
**Description:** The application needs to be secured against potential outside threats like viruses, worms, or malware.
**Rationale:** Users need to be able to have confidence that the application will not give them malware.
**Fit Criterion:** The user is safe when they download the application.
**Priority:** High priority.
**Dependencies:** ID# 17.

- **ID#:** 20 **Type:** Legal Compliance
**Description:** It is important for this project to have the appropriate legal compliance by respecting intellectual property, data protection, security, and consumer protection.
**Rationale:** The project need to operate within the bounds of the law, especially if there are plans to release it to the public for consumer use.
**Fit Criterion:** The project aligns with reputable sources for legal compliance such as the US Federal Trade Commission (FTC) or the World Intellectual Property Organization (WIPO).
**Priority:** This is of lower priority because I may not have the extra time, and probably do not plan on releasing the project to the public.
**Dependencies:** ID# 17, 18, 19.

